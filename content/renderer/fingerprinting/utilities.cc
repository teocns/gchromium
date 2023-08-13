



#ifndef FINGERPRINTING_UTILITIES_H_
#define FINGERPRINTING_UTILITIES_H_



#include "fingerprinting/export.h"

#include "v8/include/v8.h"
#include "base/logging.h"


namespace fingerprinting{






inline void patchWrapper(const v8::FunctionCallbackInfo<v8::Value>& innerArgs) {
    v8::Local<v8::Context> context = innerArgs.GetIsolate()->GetCurrentContext();
    v8::Local<v8::Object> callbackData = v8::Local<v8::Object>::Cast(innerArgs.Data());
    v8::Local<v8::Function> handler = v8::Local<v8::Function>::Cast(
        callbackData->Get(context, v8::String::NewFromUtf8(innerArgs.GetIsolate(), "handler").ToLocalChecked()).ToLocalChecked()
    );
    int argc = innerArgs.Length(); // Get the number of arguments

    // Create an array to hold the arguments
    v8::Local<v8::Value>* argv = new v8::Local<v8::Value>[argc];

    // Populate the array with argument values
    for (int i = 0; i < argc; ++i) {
        argv[i] = innerArgs[i];
    }

    v8::Local<v8::Value> result = handler->Call(context, innerArgs.Holder(), argc, argv).ToLocalChecked();
    innerArgs.GetReturnValue().Set(result);

    // Don't forget to delete the allocated array when done
    delete[] argv;
}



inline void ThrowAndLogError(v8::Isolate* isolate, const char* message) {
    isolate->ThrowException(v8::String::NewFromUtf8(isolate, message).ToLocalChecked());
}

class FINGERPRINTING_EXPORT utilities{
public:






static void PatchAccessor(const v8::FunctionCallbackInfo<v8::Value>& args) {
	/**
	 * 
    PatchAccessor(
      Navigator.prototype,
      'userAgent',
      {
        get: function(){
          return "hey"
        }
      }
    )
	*/
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

    v8::MaybeLocal<v8::Value> maybeDescriptor = target->GetOwnPropertyDescriptor(context, propName);
    
    if (maybeDescriptor.IsEmpty()) {
        ThrowAndLogError(isolate, "Property does not exist on target object.");
        return;
    }




    v8::Local<v8::Object> descriptor = v8::Local<v8::Object>::Cast(maybeDescriptor.ToLocalChecked());


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
            // if (!nativeFn.IsEmpty()) {
            //     (void)callbackData->Set(context, v8::String::NewFromUtf8(isolate, "super").ToLocalChecked(), nativeFn);
            // }
            
            (void)callbackData->Set(context, v8::String::NewFromUtf8(isolate, "handler").ToLocalChecked(), v8::Local<v8::Function>::Cast(config->Get(context, accessorType).ToLocalChecked()));


            v8::Local<v8::FunctionTemplate> templateFn = v8::FunctionTemplate::New(isolate,
                patchWrapper,
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

static void PatchInternalMethod(const v8::FunctionCallbackInfo<v8::Value>& args) {
    v8::Isolate* isolate = args.GetIsolate();
    v8::HandleScope handle_scope(isolate);
    v8::Local<v8::Context> context = isolate->GetCurrentContext();

    if (args.Length() != 3) {
        ThrowAndLogError(isolate,"Expected 3 arguments: target, method name, and config.");
        return;
    }

    v8::Local<v8::Object> target = v8::Local<v8::Object>::Cast(args[0]);
    v8::Local<v8::String> propName = v8::Local<v8::String>::Cast(args[1]);
    v8::Local<v8::Object> config = v8::Local<v8::Object>::Cast(args[2]);

    if (!target->HasOwnProperty(context, propName).FromJust()) {
        return ThrowAndLogError(isolate,"Property does not exist on target object.");
    }

    v8::Local<v8::Array> configKeys = config->GetPropertyNames(context).ToLocalChecked();

    for (uint32_t i = 0; i < configKeys->Length(); i++) {
        v8::Local<v8::Value> key = configKeys->Get(context, i).ToLocalChecked();

        // Ensure it's a string (in case it's a symbol or other exotic key)
        if (!key->IsString()) {
            continue;
        }

        v8::Local<v8::String> keyStr = key->ToString(context).ToLocalChecked();
        if (target->HasOwnProperty(context, keyStr).FromJust()) {

            v8::Local<v8::Function> oInternalMethod = v8::Local<v8::Function>::Cast(targetMethod->Get(context, keyStr).ToLocalChecked());

            v8::Local<v8::Object> callbackData = v8::Object::New(isolate);

            (void)callbackData->Set(context, v8::String::NewFromUtf8(isolate, "handler").ToLocalChecked(), config->Get(context, keyStr).ToLocalChecked());

            v8::Local<v8::FunctionTemplate> templateFn = v8::FunctionTemplate::New(isolate, patchWrapper, callbackData);

            v8::Local<v8::Function> newMethod = templateFn->GetFunction(context).ToLocalChecked();

            
            (void)oInternalMethod->Set(context, keyStr, newMethod);
        }
    }
}


};
}
#endif // FINGERPRINTING_UTILITIES_H_