



#ifndef FINGERPRINTING_UTILITIES_H_
#define FINGERPRINTING_UTILITIES_H_




#include "v8.h"


#include "v8-internal.h"








namespace fingerprinting{






inline void patchWrapper(const v8::FunctionCallbackInfo<v8::Value>& innerArgs) {
    v8::Isolate* isolate = innerArgs.GetIsolate();
    v8::Local<v8::Context> context = innerArgs.GetIsolate()->GetCurrentContext();
    v8::Local<v8::Object> callbackData = v8::Local<v8::Object>::Cast(innerArgs.Data());
    

    v8::Local<v8::Value> valueOverride = callbackData->Get(context, v8::String::NewFromUtf8(isolate, "handler").ToLocalChecked()).ToLocalChecked();


    if (!valueOverride->IsFunction()) {
        innerArgs.GetReturnValue().Set(valueOverride);
        return;
    }

    // Target will be passed for getter/setter functions. We want to pass it as the first argument
    v8::MaybeLocal<v8::Value> maybeTarget = callbackData->Get(context, v8::String::NewFromUtf8(isolate, "target").ToLocalChecked());

    const bool hasTarget = !maybeTarget.IsEmpty();
    const int argc = innerArgs.Length() + (hasTarget ? 1 : 0);

    v8::Local<v8::Value> argv[argc];


    int argvStartIdx = 0;
    if (hasTarget) {
        v8::Local<v8::Value> target = maybeTarget.ToLocalChecked();
        argv[0] = target;
        argvStartIdx = 1;
    }
    

    for (int i = argvStartIdx; i < argc; ++i) {
        argv[i] = innerArgs[i];
    }

    

    


    v8::Local<v8::Function> handler = v8::Local<v8::Function>::Cast(valueOverride);



    // argv[0] = target;
    // // Get the "thisArg" object so that it is passed as second parameter.
    // // Similar as you would get when calling Function.prototype.apply(thisArg, [arguments])

    // argv[1] = innerArgs.This();

    // Populate the array with argument values
    

    v8::MaybeLocal<v8::Value> maybeResult = handler->Call(context, innerArgs.Holder(), argc, argv);
    
    if (!maybeResult.IsEmpty()) {
        // If the result is empty, an exception has occurred,
        // so we need to handle the exception
        innerArgs.GetReturnValue().Set(maybeResult.ToLocalChecked());    
    }
    else{
        innerArgs.GetReturnValue().SetUndefined();
    }
}



inline void ThrowAndLogError(v8::Isolate* isolate, const char* message) {
    isolate->ThrowException(v8::String::NewFromUtf8(isolate, message).ToLocalChecked());
}

class utilities{
public:


static void PatchAccessor(const v8::FunctionCallbackInfo<v8::Value>& args) {
	/**
	 * 
    Patch(
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

    v8::Local<v8::Object> holder = v8::Local<v8::Object>::Cast(ValidateArg(0, true, "Expected first argument to be an object (holder)."));
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

    v8::MaybeLocal<v8::Value> maybeDescriptor = holder->GetOwnPropertyDescriptor(context, propName);
    
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
            (void)callbackData->Set(context, v8::String::NewFromUtf8(isolate, "target").ToLocalChecked(), nativeFn);
            
            v8::Local<v8::FunctionTemplate> templateFn = v8::FunctionTemplate::New(isolate,
                patchWrapper,
                callbackData
            );

            templateFn->SetClassName(propName);

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



    v8::PropertyAttribute propertyAttributes = holder->GetPropertyAttributes(context, propName).FromJust();

    propertyAttributes = (v8::PropertyAttribute)(propertyAttributes | v8::PropertyAttribute::Patched);

    holder->SetAccessorProperty(propName, patchedGetterFn, patchedSetterFn, propertyAttributes, v8::DEFAULT);
}








static void PatchValue(const v8::FunctionCallbackInfo<v8::Value>& args) {
    v8::Isolate* isolate = args.GetIsolate();
    v8::HandleScope handle_scope(isolate);
    v8::Local<v8::Context> context = isolate->GetCurrentContext();

    v8::Local<v8::String> __VALUE__ = v8::String::NewFromUtf8(isolate, "value").ToLocalChecked();

    // Check for the correct number of arguments
    if (args.Length() != 3) {
        ThrowAndLogError(isolate,"Expected 3 arguments: target, property name, and value.");
        return;
    }

    // Validation for object and string arguments
    auto ValidateArg = [&](int index, bool should_be_object, const char* message) -> v8::Local<v8::Value> {
        if ((should_be_object && !args[index]->IsObject()) || (!should_be_object && !args[index]->IsString())) {
            ThrowAndLogError(isolate, message);
            return {};
        }
        return args[index];
    };

    v8::Local<v8::Object> holder = v8::Local<v8::Object>::Cast(ValidateArg(0, true, "Expected first argument to be an object (holder)."));
    v8::Local<v8::String> propName = v8::Local<v8::String>::Cast(ValidateArg(1, false, "Expected second argument to be a string (property name)."));
    v8::Local<v8::Value> handler = args[2];

    v8::MaybeLocal<v8::Value> maybeDescriptor = holder->GetOwnPropertyDescriptor(context, propName);
    
    if (maybeDescriptor.IsEmpty()) {
        ThrowAndLogError(isolate, "Property does not exist on target object.");
        return;
    }

    

    v8::Local<v8::Object> descriptor = v8::Local<v8::Object>::Cast(maybeDescriptor.ToLocalChecked());

    // Check that the descriptor is a data descriptor. Must have a .value

    if (!descriptor->Has(context, __VALUE__).FromJust()) {
        ThrowAndLogError(isolate, "Cannot patch an accessor descriptor with this PatchValue. Use PatchAccessor instead.");
        return;
    }


    // The target object being patched (the property of the holder)
    v8::Local<v8::Value> target = descriptor->Get(context, __VALUE__).ToLocalChecked();


    


    if (!handler->IsFunction()) {
        ThrowAndLogError(isolate, "Expected third argument to be a function.");
        return;
    }




    v8::Local<v8::Object> wrapperInnerArgs = v8::Object::New(isolate);

    
    (void)wrapperInnerArgs->Set(context, v8::String::NewFromUtf8(isolate, "handler").ToLocalChecked(), handler).ToChecked();
    (void)wrapperInnerArgs->Set(context, v8::String::NewFromUtf8(isolate, "holder").ToLocalChecked(), holder).ToChecked();
    (void)wrapperInnerArgs->Set(context, v8::String::NewFromUtf8(isolate, "target").ToLocalChecked(), target).ToChecked();


    v8::PropertyAttribute propertyAttributes = holder->GetPropertyAttributes(context, propName).FromJust();

    // Assign v8::PropertyAttribute::Patched to propertyAttributes

    propertyAttributes = (v8::PropertyAttribute)(propertyAttributes | v8::PropertyAttribute::Patched);


    
    // Re-apply with the original property attributes
    v8::Maybe<bool> maybesucceess = holder->SetAccessor(
        context,
        propName,
        AccessorGetterWrapper,
        AccessorSetterWrapper, 
        wrapperInnerArgs,
        v8::DEFAULT,
        propertyAttributes,
        v8::SideEffectType::kHasSideEffect,
        v8::SideEffectType::kHasSideEffect
    );


    bool success= maybesucceess.FromJust();

    if (!success) {
        return;
    }

}


private:


    static void AccessorGetterWrapper(v8::Local<v8::Name> name, const v8::PropertyCallbackInfo<v8::Value>& innerArgs) {
        /**
         * 
         * The actual function() that executes when the value is accessed
         * i.e 
         * PatchValue(obj, "prop", function() { ... });
         * The last argument is this function
         * We need to invoke the handler with the following arguments:
         * 1. The original value (target)
         * 2. 
        */
        v8::Isolate* isolate = innerArgs.GetIsolate();
        v8::HandleScope handle_scope(isolate);
        v8::Local<v8::Context> context = isolate->GetCurrentContext();
        v8::Local<v8::Object> callbackData = v8::Local<v8::Object>::Cast(innerArgs.Data());

        
        // The user-defined handler. Might return a primitive, or a function.
        v8::Local<v8::Function> handler = v8::Local<v8::Function>::Cast(
            callbackData->Get(context, v8::String::NewFromUtf8(isolate, "handler").ToLocalChecked()).ToLocalChecked()
        );


        // The target patched
        // In javascript, this is commonly referred to as "target"
        v8::Local<v8::Value> target = callbackData->Get(context, v8::String::NewFromUtf8(isolate, "target").ToLocalChecked()).ToLocalChecked();

        const int argc=3;

        v8::Local<v8::Value> argv[argc] = {
            target,
            innerArgs.This(),
            name
        };

        v8::MaybeLocal<v8::Value> maybeResult = handler->Call(context, innerArgs.Holder(), argc, argv);

        if (maybeResult.IsEmpty()) {
            innerArgs.GetReturnValue().SetUndefined();
            return;
        }


        // If the result is a function, we need to wrap it again to ensure that it is masked (native function)


        v8::Local<v8::Value> result = maybeResult.ToLocalChecked();


        // If the result is a function, we need to wrap it again to ensure that it is masked (native function)

        if (result->IsFunction()) {

            // Create new callback data
            v8::Local<v8::Object> innerFnCallbackData = v8::Object::New(isolate);
            (void)innerFnCallbackData->Set(context, v8::String::NewFromUtf8(isolate, "handler").ToLocalChecked(), result).ToChecked();
            // (void)innerFnCallbackData->Set(context, v8::String::NewFromUtf8(isolate, "target").ToLocalChecked(), target).ToChecked();

            v8::Local<v8::FunctionTemplate> templateFn = v8::FunctionTemplate::New(
                isolate,
                patchWrapper,
                innerFnCallbackData
            );

            // set classname from the `v8::Local<v8::Name> name` parameter. Convert it to local<stirng>
            v8::Local<v8::String> funcName = v8::Local<v8::String>::Cast(
                name
            );

            templateFn->SetClassName(funcName);

            innerArgs.GetReturnValue().Set(templateFn->GetFunction(context).ToLocalChecked());
            return;
        }

        innerArgs.GetReturnValue().Set(result);


    }

    static void AccessorSetterWrapper(v8::Local<v8::Name> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info) {
        
    }
    


};
}
#endif // FINGERPRINTING_UTILITIES_H_