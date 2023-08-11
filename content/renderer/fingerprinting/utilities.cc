



#ifndef FINGERPRINTING_UTILITIES_H_
#define FINGERPRINTING_UTILITIES_H_



#include "fingerprinting/export.h"

#include "v8/include/v8.h"
#include "base/logging.h"


namespace fingerprinting{



inline void accessorWrapper(const v8::FunctionCallbackInfo<v8::Value>& innerArgs){


    v8::Local<v8::Context> context = innerArgs.GetIsolate()->GetCurrentContext();
    v8::Local<v8::Object> callbackData = v8::Local<v8::Object>::Cast(innerArgs.Data());
    v8::Local<v8::Function> handler = v8::Local<v8::Function>::Cast(callbackData->Get(context, v8::String::NewFromUtf8(innerArgs.GetIsolate(), "handler").ToLocalChecked()).ToLocalChecked());


    v8::Local<v8::Value> origValue = callbackData->Get(context, v8::String::NewFromUtf8(innerArgs.GetIsolate(), "super").ToLocalChecked()).ToLocalChecked();

    v8::Local<v8::Object> target = innerArgs.Holder();
    v8::Local<v8::Object> thisArg = innerArgs.This();
    v8::Local<v8::Value> argv[3] = { target, thisArg, origValue };

    v8::Local<v8::Value> result = handler->Call(context, innerArgs.Holder(), 3, argv).ToLocalChecked();

    // if (result->IsFunction()) {
    // 	innerArgs.GetReturnValue().Set(
    // 		maskRetVal(result, context)
    // 	);
    // 	return;
    // }

    innerArgs.GetReturnValue().Set(result);
}

inline void ThrowAndLogError(v8::Isolate* isolate, const char* message) {
    isolate->ThrowException(v8::String::NewFromUtf8(isolate, message).ToLocalChecked());
}

class FINGERPRINTING_EXPORT utilities{
public:






static void PatchAccessor(const v8::FunctionCallbackInfo<v8::Value>& args) {
    v8::Isolate* isolate = args.GetIsolate();
    v8::HandleScope handle_scope(isolate);
    v8::Local<v8::Context> context = isolate->GetCurrentContext();

    v8::Local<v8::String> __SET__ = v8::String::NewFromUtf8(isolate, "set").ToLocalChecked();
    v8::Local<v8::String> __GET__ = v8::String::NewFromUtf8(isolate, "get").ToLocalChecked();

    // Check for the correct number of arguments
    if (args.Length() != 3) {
        ThrowAndLogError(isolate,"Expected 3 arguments: target, property name, and config.");
        return;
    }

    // Validation for object and string arguments
    auto ValidateArg = [&](int index, bool should_be_object, const char* message) -> v8::Local<v8::Value> {
        if ((should_be_object && !args[index]->IsObject()) || (!should_be_object && !args[index]->IsString())) {
            ThrowAndLogError(isolate,message);
            return {};
        }
        return args[index];
    };

    v8::Local<v8::Object> target = v8::Local<v8::Object>::Cast(ValidateArg(0, true, "Expected first argument to be an object (target)."));
    v8::Local<v8::String> propName = v8::Local<v8::String>::Cast(ValidateArg(1, false, "Expected second argument to be a string (property name)."));
    v8::Local<v8::Object> config = v8::Local<v8::Object>::Cast(ValidateArg(2, true, "Expected third argument to be an object (config)."));

    // Check if config object has a getter/setter and validate its type
    auto HasValidFunction = [&](v8::Local<v8::String> key, const char* message) {
        if (config->HasOwnProperty(context, key).FromJust() && !config->Get(context, key).ToLocalChecked()->IsFunction()) {
            ThrowAndLogError(isolate,message);
            return false;
        }
        return true;
    };

    if (!HasValidFunction(__GET__, "'get' property in config should be a function.") ||
        !HasValidFunction(__SET__, "'set' property in config should be a function.")) {
        return;
    }


		v8::Local<v8::Value> descValue = target->GetOwnPropertyDescriptor(context, propName).ToLocalChecked();
    v8::Local<v8::Object> descriptor = v8::Local<v8::Object>::Cast(descValue);


    // Function to process either a getter or setter
    auto ProcessAccessor = [&](v8::Local<v8::String> accessorType) -> v8::Local<v8::Function> {
        v8::Local<v8::Function> nativeFn;
        if (descriptor->Has(context, accessorType).FromJust()) {
            v8::Local<v8::Value> value = descriptor->Get(context, accessorType).ToLocalChecked();
            if (value->IsFunction()) {
                nativeFn = v8::Local<v8::Function>::Cast(value);
            }
        }

        if (config->HasOwnProperty(context, accessorType).FromJust()) {
            v8::Local<v8::Object> callbackData = v8::Object::New(isolate);
            if (!nativeFn.IsEmpty()) {
                (void)callbackData->Set(context, v8::String::NewFromUtf8(isolate, "super").ToLocalChecked(), nativeFn);
            }
            
						(void)callbackData->Set(context, v8::String::NewFromUtf8(isolate, "handler").ToLocalChecked(), v8::Local<v8::Function>::Cast(config->Get(context, accessorType).ToLocalChecked()));


            v8::Local<v8::FunctionTemplate> templateFn = v8::FunctionTemplate::New(isolate,
                accessorWrapper,
                callbackData
            );

            return templateFn->GetFunction(context).ToLocalChecked();
        }
        return nativeFn;
    };

    if (!descriptor->Has(context, __GET__).FromJust() && !descriptor->Has(context, __SET__).FromJust()) {
        ThrowAndLogError(isolate,"Cannot patch a data descriptor with this PatchAccessor. Use PatchValue instead.");
        return;
    }

    v8::Local<v8::Function> patchedGetterFn = ProcessAccessor(__GET__);
    v8::Local<v8::Function> patchedSetterFn = ProcessAccessor(__SET__);

    target->SetAccessorProperty(propName, patchedGetterFn, patchedSetterFn, target->GetPropertyAttributes(context, propName).FromJust(), v8::DEFAULT);
}


private:
	// static v8::Local<v8::Function> maskRetVal(v8::Local<v8::Value> result, v8::Local<v8::Context> context){
	// 	return v8::Function::New(context, 
	// 			maskRetValWrapper,
	// 			result
	// 	).ToLocalChecked();

	// }

	// static void maskRetValWrapper(const v8::FunctionCallbackInfo<v8::Value>& nativeArgs) {
	// 		v8::Local<v8::Function> innerFunc = v8::Local<v8::Function>::Cast(nativeArgs.Data());
	// 		v8::Local<v8::Value> innerResult = innerFunc->Call(nativeArgs.GetIsolate()->GetCurrentContext(), nativeArgs.Holder(), 0, nullptr).ToLocalChecked();
	// 		nativeArgs.GetReturnValue().Set(innerResult);
	// }



	// get_str = 



};
}
#endif // FINGERPRINTING_UTILITIES_H_