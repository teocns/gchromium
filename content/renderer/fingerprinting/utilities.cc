



#ifndef FINGERPRINTING_UTILITIES_H_
#define FINGERPRINTING_UTILITIES_H_



#include "fingerprinting/export.h"

#include "v8/include/v8.h"
#include "base/logging.h"


namespace fingerprinting{


class FINGERPRINTING_EXPORT utilities{
public:
// Internal method	Corresponding trap
// [[GetPrototypeOf]]	getPrototypeOf()
// [[SetPrototypeOf]]	setPrototypeOf()
// [[IsExtensible]]	isExtensible()
// [[PreventExtensions]]	preventExtensions()
// [[GetOwnProperty]]	getOwnPropertyDescriptor()
// [[DefineOwnProperty]]	defineProperty()
// [[HasProperty]]	has()
// [[Get]]	get()
// [[Set]]	set()
// [[Delete]]	deleteProperty()
// [[OwnPropertyKeys]]	ownKeys()
// Function objects also have the following internal methods:

// Internal method	Corresponding trap
// [[Call]]	apply()
// [[Construct]]	construct()


static void wrappedFunctionCallback(const v8::FunctionCallbackInfo<v8::Value>& innerArgs) {
    v8::Isolate* innerIsolate = innerArgs.GetIsolate();
    v8::Local<v8::Context> innerContext = innerIsolate->GetCurrentContext();
    
    
    

    // get handlerFn, target, propName from data
    // beware of error: no member named 'Get' in 'v8::Value'
    // cast dataVal to object:

    v8::Local<v8::Value> dataVal = innerArgs.Data();

    v8::Local<v8::Object> data = v8::Local<v8::Object>::Cast(dataVal);

    
    v8::Local<v8::Object> target = v8::Local<v8::Object>::Cast(data->Get(innerContext, v8::String::NewFromUtf8(innerIsolate, "target").ToLocalChecked()).ToLocalChecked());
    v8::Local<v8::Function> nativeFunc = v8::Local<v8::Function>::Cast(data->Get(innerContext, v8::String::NewFromUtf8(innerIsolate, "nativeFunc").ToLocalChecked()).ToLocalChecked());
    v8::Local<v8::Function> handlerFn = v8::Local<v8::Function>::Cast(data->Get(innerContext, v8::String::NewFromUtf8(innerIsolate, "handlerFn").ToLocalChecked()).ToLocalChecked());
    v8::Local<v8::String> propName = v8::Local<v8::String>::Cast(data->Get(innerContext, v8::String::NewFromUtf8(innerIsolate, "propName").ToLocalChecked()).ToLocalChecked());
    v8::Local<v8::String> trapName = v8::Local<v8::String>::Cast(data->Get(innerContext, v8::String::NewFromUtf8(innerIsolate, "trapName").ToLocalChecked()).ToLocalChecked());
    std::string trapNameStr = *v8::String::Utf8Value(innerIsolate, trapName);

    int len = innerArgs.Length();
    std::vector<v8::Local<v8::Value>> innerArgsVec(len);
    for (int i = 0; i < len; i++) {
        innerArgsVec[i] = innerArgs[i];
    }

    v8::Local<v8::Value> origResult = v8::Undefined(innerIsolate);

    if (trapNameStr == "apply") {
        v8::Local<v8::Value> applyArgs[] = { nativeFunc, innerArgs.This(), origResult, nativeFunc };
        origResult = handlerFn->Call(innerContext, handlerFn, 3, applyArgs).ToLocalChecked();
    } else if (trapNameStr == "get") {
        v8::Local<v8::Value> getArgs[] = { target, propName, nativeFunc};
        origResult = handlerFn->Call(innerContext, handlerFn, 2, getArgs).ToLocalChecked();
    } else if (trapNameStr == "set") {
        v8::Local<v8::Value> setArgs[] = { target, propName, origResult, nativeFunc };
        origResult = handlerFn->Call(innerContext, handlerFn, 3, setArgs).ToLocalChecked();
    } else if (trapNameStr == "construct") {
        v8::Local<v8::Value> constructArgs[] = { nativeFunc, innerArgs.This(), nativeFunc };
        origResult = handlerFn->Call(innerContext, handlerFn, 2, constructArgs).ToLocalChecked();
    } else {
        // Handle unknown trapName if necessary
        LOG(WARNING) << "Unknown trapName: " << trapNameStr;
    }


    innerArgs.GetReturnValue().Set(origResult);
}





static void MockTrap(const v8::FunctionCallbackInfo<v8::Value>& args) {
/**
 * Method commonly used to override objects' internal method traps
  MockTrap(NavigatorUAData.prototype, 'getHighEntropyValues', 'apply', (target, thisArg, args) => {
    return "Hello!"
  });
*/
    v8::Isolate* isolate = args.GetIsolate();
    v8::HandleScope handle_scope(isolate);
    v8::Local<v8::Context> context = isolate->GetCurrentContext();

      
    v8::Local<v8::Object> target = v8::Local<v8::Object>::Cast(args[0]);
    v8::Local<v8::String> propName = v8::Local<v8::String>::Cast(args[1]);
    // The trap name
    v8::Local<v8::String> trapName = v8::Local<v8::String>::Cast(args[2]);
    v8::Local<v8::Function> handlerFn = v8::Local<v8::Function>::Cast(args[3]);




    v8::Local<v8::Object> descriptor;
    v8::Local<v8::Function> nativeFunc;



    v8::MaybeLocal<v8::Value> maybeValue = target->GetOwnPropertyDescriptor(context, propName);
    if (!maybeValue.IsEmpty()) {
        v8::Local<v8::Value> value = maybeValue.ToLocalChecked();
        if (value->IsObject()) {
            descriptor = v8::Local<v8::Object>::Cast(value);
            // Now you can use 'descriptor'
        }
    }


    if (!descriptor.IsEmpty()) {

        if (!descriptor->Has(context, trapName).ToChecked()){
            // Log the trap name and the object's descriptor name
            LOG(ERROR) << "Trap " << *v8::String::Utf8Value(isolate, trapName) << " not found in descriptor " << *v8::String::Utf8Value(isolate, propName);
            return;
        }

        nativeFunc = v8::Local<v8::Function>::Cast(descriptor->Get(context, trapName).ToLocalChecked());
    }
    

    // Get the target's original function by propName
    v8::Local<v8::Object> callbackData = v8::Object::New(isolate);

    (void)callbackData->Set(context, v8::String::NewFromUtf8(isolate, "nativeFunc").ToLocalChecked(), nativeFunc);
    (void)callbackData->Set(context, v8::String::NewFromUtf8(isolate, "handlerFn").ToLocalChecked(), handlerFn);
    (void)callbackData->Set(context, v8::String::NewFromUtf8(isolate, "target").ToLocalChecked(), target);
    (void)callbackData->Set(context, v8::String::NewFromUtf8(isolate, "propName").ToLocalChecked(), propName);
    (void)callbackData->Set(context, v8::String::NewFromUtf8(isolate, "trapName").ToLocalChecked(), trapName);

    v8::Local<v8::FunctionTemplate> templ = v8::FunctionTemplate::New(isolate, wrappedFunctionCallback, callbackData);

    v8::Local<v8::Function> newFunc = templ->GetFunction(context).ToLocalChecked();

    // Replace the original function with the new function on the target object
    


    v8::Maybe<bool> maybeDescriptorSetSuccess = v8::Nothing<bool>();


     // Check if we're dealing with a getter or setter, and adjust accordingly
    if (trapName->StrictEquals(v8::String::NewFromUtf8(isolate, "get").ToLocalChecked())) {
        descriptor->SetAccessorProperty(propName, newFunc, v8::Local<v8::Function>(), v8::ReadOnly);
    } else if (trapName->StrictEquals(v8::String::NewFromUtf8(isolate, "set").ToLocalChecked())) {
        descriptor->SetAccessorProperty(propName, v8::Local<v8::Function>(), newFunc, v8::ReadOnly);
    } else {
        maybeDescriptorSetSuccess = descriptor->Set(context, trapName, newFunc);
    }

    if (!maybeDescriptorSetSuccess.IsNothing() && !maybeDescriptorSetSuccess.FromJust()) {
        LOG(ERROR) << "Failed to replace trap on descriptor";
        return;
    }

    if (!target->DefineOwnProperty(context, propName, descriptor).FromJust()){
        LOG(ERROR) << "Failed to replace target's descriptor";
        return;
    }
}




};
}
#endif // FINGERPRINTING_UTILITIES_H_