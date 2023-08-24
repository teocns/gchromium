



#ifndef FINGERPRINTING_UTILITIES_H_
#define FINGERPRINTING_UTILITIES_H_




#include "v8.h"


#include "v8-internal.h"

#include <set>
#include <string>
#include "base/logging.h"






namespace fingerprinting{


using namespace v8;


using V8String = v8::String;





inline void patchWrapper(const FunctionCallbackInfo<Value>& innerArgs) {
    Isolate* isolate = innerArgs.GetIsolate();
    Local<Context> context = innerArgs.GetIsolate()->GetCurrentContext();
    Local<Object> callbackData = Local<Object>::Cast(innerArgs.Data());
    

    Local<Value> valueOverride = callbackData->Get(context, V8String::NewFromUtf8(isolate, "handler").ToLocalChecked()).ToLocalChecked();


    if (!valueOverride->IsFunction()) {
        innerArgs.GetReturnValue().Set(valueOverride);
        return;
    }

    // Target will be passed for getter/setter functions. We want to pass it as the first argument
    Local<Value> target = callbackData->Get(context, V8String::NewFromUtf8(isolate, "target").ToLocalChecked()).ToLocalChecked();


    const int argc = innerArgs.Length() + 2;

    Local<Value> argv[argc];

    argv[0] = target;
    argv[1] = innerArgs.This();
    
    for (int i = 2; i < argc; ++i) {
        argv[i] = innerArgs[i];
    }

    

    


    Local<Function> handler = Local<Function>::Cast(valueOverride);
    

    MaybeLocal<Value> maybeResult = handler->Call(context, innerArgs.Holder(), argc, argv);
    
    if (!maybeResult.IsEmpty()) {
        // If the result is empty, an exception has occurred,
        // so we need to handle the exception
        innerArgs.GetReturnValue().Set(maybeResult.ToLocalChecked());    
    }
    else{
        innerArgs.GetReturnValue().SetUndefined();
    }
}



inline void ThrowAndLogError(Isolate* isolate, const char* message) {
    isolate->ThrowException(V8String::NewFromUtf8(isolate, message).ToLocalChecked());
}




inline void CopyOwnPropertiesDescriptors(Local<Object> source, Local<Object> target, Local<Context> context, Isolate* isolate) {
    /**
     * Copies own property descriptors
     * 
     * 
     * Object.getOwnPropertyDescriptors(Function.prototype.toString)
        {
            length: {value: 0, writable: false, enumerable: false, configurable: true},
            name: {value: 'toString', writable: false, enumerable: false, configurable: true}
        }
    */
    
    Local<V8String> __VALUE__ = V8String::NewFromUtf8(isolate, "value").ToLocalChecked();
    Local<V8String> __GET__ = V8String::NewFromUtf8(isolate, "get").ToLocalChecked();
    Local<V8String> __SET__ = V8String::NewFromUtf8(isolate, "set").ToLocalChecked();



    Local<Array> sourceProps = source->GetOwnPropertyNames(context,static_cast<PropertyFilter>(0)).ToLocalChecked();

    // Iterate through the properties of the source object
    for (uint32_t i = 0; i < sourceProps->Length(); i++) {

        // Get the descriptor's property name
        Local<Value> key = sourceProps->Get(context, i).ToLocalChecked();

        Local<Name> name = Local<Name>::Cast(key);

        

        MaybeLocal<Value> maybeSrcPropDescr = source->GetOwnPropertyDescriptor(context,name).ToLocalChecked();
        

        if (maybeSrcPropDescr.IsEmpty()){
            continue;
        }
        
        PropertyAttribute srcPropDescrAttrs = source->GetPropertyAttributes(context, name).FromJust();

        // The source's own property descriptor in scope
        Local<Object> srcPropDescr = Local<Object>::Cast(maybeSrcPropDescr.ToLocalChecked());




        if (srcPropDescr->Has(context,__VALUE__).ToChecked()){
            // We're dealing with a data descriptor
            PropertyDescriptor newDescriptor(
                srcPropDescr->Get(context,__VALUE__).ToLocalChecked(),
                (srcPropDescrAttrs & PropertyAttribute::ReadOnly) == 0 // Is Writable?
            );

            newDescriptor.set_enumerable((srcPropDescrAttrs & PropertyAttribute::DontEnum) == 0);
            newDescriptor.set_configurable((srcPropDescrAttrs & PropertyAttribute::DontDelete) == 0);

            (void)target->DefineProperty(
                context, name, newDescriptor
            );
        }
        else{
            // We're dealing with an accessor descriptor
            PropertyDescriptor newDescriptor(
                srcPropDescr->Get(context,__GET__).ToLocalChecked(),
                srcPropDescr->Get(context,__SET__).ToLocalChecked()
            );

            newDescriptor.set_enumerable((srcPropDescrAttrs & PropertyAttribute::DontEnum) == 0);
            newDescriptor.set_configurable((srcPropDescrAttrs & PropertyAttribute::DontDelete) == 0);

            (void)target->DefineProperty(
                context, name, newDescriptor
            );
        }
        
    }

    (void)target->SetPrototype(context,source->GetPrototype());


    
}


class utilities{
public:


static void PatchAccessor(const FunctionCallbackInfo<Value>& args) {
    /**
     * Patches Accessor Descriptors
     * 
     * 

PatchAccessor(
  Navigator.prototype,
  'userAgent',
  {
    get: function(target,thisArg,...args){
      return Reflect.apply(target,thisArg,args) + " patched";
    }
  }
)

    */
    Isolate* isolate = args.GetIsolate();
    HandleScope handle_scope(isolate);
    Local<Context> context = isolate->GetCurrentContext();

    Local<V8String> __SET__ = V8String::NewFromUtf8(isolate, "set").ToLocalChecked();
    Local<V8String> __GET__ = V8String::NewFromUtf8(isolate, "get").ToLocalChecked();

    // Check for the correct number of arguments
    if (args.Length() != 3) {
        ThrowAndLogError(isolate,"Expected 3 arguments: target, property name, and value.");
        return;
    }

    // Validation for object and string arguments
    auto ValidateArg = [&](int index, bool should_be_object, const char* message) -> Local<Value> {
        if ((should_be_object && !args[index]->IsObject()) || (!should_be_object && !args[index]->IsString())) {
            ThrowAndLogError(isolate, message);
            return {};
        }
        return args[index];
    };

    Local<Object> holder = Local<Object>::Cast(ValidateArg(0, true, "Expected first argument to be an object (holder)."));
    Local<V8String> propName = Local<V8String>::Cast(ValidateArg(1, false, "Expected second argument to be a string (property name)."));
    Local<Object> config = Local<Object>::Cast(ValidateArg(2, true, "Expected third argument to be an object (config)."));

    
    // Check if config object has a getter/setter and validate its type
    auto HasValidFunction = [&](Local<V8String> key, const char* message) {
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

    MaybeLocal<Value> maybeDescriptor = holder->GetOwnPropertyDescriptor(context, propName);
    
    if (maybeDescriptor.IsEmpty()) {
        ThrowAndLogError(isolate, "Property does not exist on target object.");
        return;
    }

    

    Local<Object> descriptor = Local<Object>::Cast(maybeDescriptor.ToLocalChecked());

    // Function to process either a getter or setter
    auto ProcessAccessor = [&](Local<V8String> accessorType) -> Local<Function> {
        Local<Function> nativeFn;
        if (descriptor->Has(context, accessorType).FromJust()) {
            Local<Value> value = descriptor->Get(context, accessorType).ToLocalChecked();
            if (value->IsFunction()) {
                nativeFn = Local<Function>::Cast(value);
            }
            else{
                return nativeFn; // Returns an undefined function
            }
        }

        if (config->HasOwnProperty(context, accessorType).FromJust()) {
            Local<Object> callbackData = Object::New(isolate);
            // if (!nativeFn.IsEmpty()) {
            //     (void)callbackData->Set(context, V8String::NewFromUtf8(isolate, "super").ToLocalChecked(), nativeFn);
            // }



            
            (void)callbackData->Set(context, V8String::NewFromUtf8(isolate, "handler").ToLocalChecked(), Local<Function>::Cast(config->Get(context, accessorType).ToLocalChecked()));
            (void)callbackData->Set(context, V8String::NewFromUtf8(isolate, "target").ToLocalChecked(), nativeFn);
            
            Local<FunctionTemplate> templateFn = FunctionTemplate::New(
                isolate,
                patchWrapper,
                callbackData
            );
            templateFn->RemovePrototype();
            templateFn->SetClassName(propName);


            Local<Function> fnWrapped = templateFn->GetFunction(context).ToLocalChecked();
            
            CopyOwnPropertiesDescriptors(nativeFn, fnWrapped, context, isolate);

            return fnWrapped;
            
        }
        return nativeFn;
    };

    if (!descriptor->Has(context, __GET__).FromJust() && !descriptor->Has(context, __SET__).FromJust()) {
        ThrowAndLogError(isolate,"Cannot patch a data descriptor with this PatchAccessor. Use PatchValue instead.");
        return;
    }

    Local<Function> patchedGetterFn = ProcessAccessor(__GET__);
    Local<Function> patchedSetterFn = ProcessAccessor(__SET__);



    PropertyAttribute srcPropAttrs = holder->GetPropertyAttributes(context, propName).FromJust();

    PropertyDescriptor newDescriptor(patchedGetterFn,patchedSetterFn);

    newDescriptor.set_enumerable((srcPropAttrs & PropertyAttribute::DontEnum) == 0);
    newDescriptor.set_configurable((srcPropAttrs & PropertyAttribute::DontDelete) == 0);
    

    // Will always succeeed because of Patched
    (void)holder->DefineProperty(
        context, propName, newDescriptor
    );

}



static void PatchValue(const FunctionCallbackInfo<Value>& args) {
    /**
     * Patches Data Descriptors
     * 
     * 
PatchValue(
    Function.prototype,
    'toString',
    function(){
      return "hello";
    }   
)
    */
    Isolate* isolate = args.GetIsolate();
    HandleScope handle_scope(isolate);
    Local<Context> context = isolate->GetCurrentContext();

    Local<V8String> __VALUE__ = V8String::NewFromUtf8(isolate, "value").ToLocalChecked();

    // Check for the correct number of arguments
    if (args.Length() != 3) {
        ThrowAndLogError(isolate,"Expected 3 arguments: target, property name, and value.");
        return;
    }

    // Validation for object and string arguments
    auto ValidateArg = [&](int index, bool should_be_object, const char* message) -> Local<Value> {
        if ((should_be_object && !args[index]->IsObject()) || (!should_be_object && !args[index]->IsString())) {
            ThrowAndLogError(isolate, message);
            return {};
        }
        return args[index];
    };

    Local<Object> holder = Local<Object>::Cast(ValidateArg(0, true, "Expected first argument to be an object (holder)."));
    Local<V8String> propName = Local<V8String>::Cast(ValidateArg(1, false, "Expected second argument to be a string (property name)."));
    Local<Value> handler = args[2];

    MaybeLocal<Value> maybeDescriptor = holder->GetOwnPropertyDescriptor(context, propName);
    
    if (maybeDescriptor.IsEmpty()) {
        ThrowAndLogError(isolate, "Property does not exist on target object.");
        return;
    }

    

    Local<Object> descriptor = Local<Object>::Cast(maybeDescriptor.ToLocalChecked());

    // Check that the descriptor is a data descriptor. Must have a .value

    if (!descriptor->Has(context, __VALUE__).FromJust()) {
        ThrowAndLogError(isolate, "Cannot patch an accessor descriptor with this PatchValue. Use PatchAccessor instead.");
        return;
    }


    // The target object being patched (the property of the holder)
    Local<Value> target = descriptor->Get(context, __VALUE__).ToLocalChecked();

    


    if (!handler->IsFunction()) {
        ThrowAndLogError(isolate, "Expected third argument to be a function.");
        return;
    }




    Local<Object> wrapperInnerArgs = Object::New(isolate);

    
    (void)wrapperInnerArgs->Set(context, V8String::NewFromUtf8(isolate, "handler").ToLocalChecked(), handler).ToChecked();
    (void)wrapperInnerArgs->Set(context, V8String::NewFromUtf8(isolate, "holder").ToLocalChecked(), holder).ToChecked();
    (void)wrapperInnerArgs->Set(context, V8String::NewFromUtf8(isolate, "target").ToLocalChecked(), target).ToChecked();


    Local<Object> oTarget = Local<Object>::Cast(target);



    Local<FunctionTemplate> templateFn = FunctionTemplate::New(
        isolate,
        patchWrapper,
        wrapperInnerArgs
    );

    templateFn->RemovePrototype();
    templateFn->SetClassName(propName);
    
    Local<Function> fnWrapped = templateFn->GetFunction(context).ToLocalChecked();

    CopyOwnPropertiesDescriptors(oTarget,fnWrapped,context,isolate);



    PropertyAttribute targetPropertyAttributes = holder->GetPropertyAttributes(context, propName).FromJust();

    // Should always evaluate to true because of Patched
    (void)holder->DefineOwnProperty(
        context, propName, fnWrapped,
        (PropertyAttribute) (targetPropertyAttributes | PropertyAttribute::Patched)
    );
}


};
}
#endif // FINGERPRINTING_UTILITIES_H_