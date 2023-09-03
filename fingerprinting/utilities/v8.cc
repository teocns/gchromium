#include "v8/include/v8.h"
#include <set>
#include <string>
#include "base/logging.h"


namespace fingerprinting {

namespace utilities {

void patchWrapper(const v8::FunctionCallbackInfo<v8::Value>& innerArgs) {
  v8::Isolate* isolate = innerArgs.GetIsolate();
  v8::Local<v8::Context> context = innerArgs.GetIsolate()->GetCurrentContext();
  v8::Local<v8::Object> callbackData =
      v8::Local<v8::Object>::Cast(innerArgs.Data());

  v8::Local<v8::Value> valueOverride =
      callbackData
          ->Get(context,
                v8::String::NewFromUtf8(isolate, "handler").ToLocalChecked())
          .ToLocalChecked();

  if (!valueOverride->IsFunction()) {
    innerArgs.GetReturnValue().Set(valueOverride);
    return;
  }

  // Target will be passed for getter/setter functions. We want to pass it as
  // the first argument
  v8::Local<v8::Value> target =
      callbackData
          ->Get(context,
                v8::String::NewFromUtf8(isolate, "target").ToLocalChecked())
          .ToLocalChecked();

  const int argc = innerArgs.Length() + 2;

  v8::Local<v8::Value> argv[argc];

  argv[0] = target;
  argv[1] = innerArgs.This();

  for (int i = 2; i < argc; ++i) {
    argv[i] = innerArgs[i];
  }

  v8::Local<v8::Function> handler = v8::Local<v8::Function>::Cast(valueOverride);

  v8::MaybeLocal<v8::Value> maybeResult =
      handler->Call(context, innerArgs.Holder(), argc, argv);

  if (!maybeResult.IsEmpty()) {
    // If the result is empty, an exception has occurred,
    // so we need to handle the exception
    innerArgs.GetReturnValue().Set(maybeResult.ToLocalChecked());
  } else {
    innerArgs.GetReturnValue().SetUndefined();
  }
}

void ThrowAndLogError(v8::Isolate* isolate, const char* message) {
  isolate->ThrowException(
      v8::String::NewFromUtf8(isolate, message).ToLocalChecked());
}

void CopyOwnPropertiesDescriptors(v8::Local<v8::Object> source,
                                  v8::Local<v8::Object> target,
                                  v8::Local<v8::Context> context,
                                  v8::Isolate* isolate) {
  /**
   * Copies own property descriptors
   *
   *
   * Object.getOwnPropertyDescriptors(Function.prototype.toString)
      {
          length: {value: 0, writable: false, enumerable: false, configurable:
   true}, name: {value: 'toString', writable: false, enumerable: false,
   configurable: true}
      }
  */

  v8::Local<v8::String> __VALUE__ =
      v8::String::NewFromUtf8(isolate, "value").ToLocalChecked();
  v8::Local<v8::String> __GET__ =
      v8::String::NewFromUtf8(isolate, "get").ToLocalChecked();
  v8::Local<v8::String> __SET__ =
      v8::String::NewFromUtf8(isolate, "set").ToLocalChecked();

  v8::Local<v8::Array> sourceProps =
      source->GetOwnPropertyNames(context, static_cast<v8::PropertyFilter>(0))
          .ToLocalChecked();

  // Iterate through the properties of the source object
  for (uint32_t i = 0; i < sourceProps->Length(); i++) {
    // Get the descriptor's property name
    v8::Local<v8::Value> key = sourceProps->Get(context, i).ToLocalChecked();

    v8::Local<v8::Name> name = v8::Local<v8::Name>::Cast(key);

    v8::MaybeLocal<v8::Value> maybeSrcPropDescr =
        source->GetOwnPropertyDescriptor(context, name).ToLocalChecked();

    if (maybeSrcPropDescr.IsEmpty()) {
      continue;
    }

    v8::PropertyAttribute srcPropDescrAttrs =
        source->GetPropertyAttributes(context, name).FromJust();

    // The source's own property descriptor in scope
    v8::Local<v8::Object> srcPropDescr =
        v8::Local<v8::Object>::Cast(maybeSrcPropDescr.ToLocalChecked());

    if (srcPropDescr->Has(context, __VALUE__).ToChecked()) {
      // We're dealing with a data descriptor
      v8::PropertyDescriptor newDescriptor(
          srcPropDescr->Get(context, __VALUE__).ToLocalChecked(),
          (srcPropDescrAttrs & v8::PropertyAttribute::ReadOnly) ==
              0  // Is Writable?
      );

      newDescriptor.set_enumerable(
          (srcPropDescrAttrs & v8::PropertyAttribute::DontEnum) == 0);
      newDescriptor.set_configurable(
          (srcPropDescrAttrs & v8::PropertyAttribute::DontDelete) == 0);

      (void)target->DefineProperty(context, name, newDescriptor);
    } else {
      // We're dealing with an accessor descriptor
      v8::PropertyDescriptor newDescriptor(
          srcPropDescr->Get(context, __GET__).ToLocalChecked(),
          srcPropDescr->Get(context, __SET__).ToLocalChecked());

      newDescriptor.set_enumerable(
          (srcPropDescrAttrs & v8::PropertyAttribute::DontEnum) == 0);
      newDescriptor.set_configurable(
          (srcPropDescrAttrs & v8::PropertyAttribute::DontDelete) == 0);

      (void)target->DefineProperty(context, name, newDescriptor);
    }
  }

  (void)target->SetPrototype(context, source->GetPrototype());
}

void PatchAccessor(const v8::FunctionCallbackInfo<v8::Value>& args) {
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
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope handle_scope(isolate);
  v8::Local<v8::Context> context = isolate->GetCurrentContext();

  v8::Local<v8::String> __SET__ =
      v8::String::NewFromUtf8(isolate, "set").ToLocalChecked();
  v8::Local<v8::String> __GET__ =
      v8::String::NewFromUtf8(isolate, "get").ToLocalChecked();

  // Check for the correct number of arguments
  if (args.Length() != 3) {
    ThrowAndLogError(isolate,
                     "Expected 3 arguments: target, property name, and value.");
    return;
  }

  // Validation for object and string arguments
  auto ValidateArg = [&](int index, bool should_be_object,
                         const char* message) -> v8::Local<v8::Value> {
    if ((should_be_object && !args[index]->IsObject()) ||
        (!should_be_object && !args[index]->IsString())) {
      ThrowAndLogError(isolate, message);
      return {};
    }
    return args[index];
  };

  v8::Local<v8::Object> holder = v8::Local<v8::Object>::Cast(ValidateArg(
      0, true, "Expected first argument to be an object (holder)."));
  v8::Local<v8::String> propName = v8::Local<v8::String>::Cast(ValidateArg(
      1, false, "Expected second argument to be a string (property name)."));
  v8::Local<v8::Object> config = v8::Local<v8::Object>::Cast(ValidateArg(
      2, true, "Expected third argument to be an object (config)."));

  // Check if config object has a getter/setter and validate its type
  auto HasValidFunction = [&](v8::Local<v8::String> key, const char* message) {
    if (config->HasOwnProperty(context, key).FromJust() &&
        !config->Get(context, key).ToLocalChecked()->IsFunction()) {
      ThrowAndLogError(isolate, message);
      return false;
    }
    return true;
  };

  if (!HasValidFunction(__GET__,
                        "'get' property in config should be a function.") ||
      !HasValidFunction(__SET__,
                        "'set' property in config should be a function.")) {
    return;
  }

  v8::MaybeLocal<v8::Value> maybeDescriptor =
      holder->GetOwnPropertyDescriptor(context, propName);

  if (maybeDescriptor.IsEmpty()) {
    ThrowAndLogError(isolate, "Property does not exist on target object.");
    return;
  }

  v8::Local<v8::Object> descriptor =
      v8::Local<v8::Object>::Cast(maybeDescriptor.ToLocalChecked());

  // Function to process either a getter or setter
  auto ProcessAccessor =
      [&](v8::Local<v8::String> accessorType) -> v8::Local<v8::Function> {
    v8::Local<v8::Function> nativeFn;
    if (descriptor->Has(context, accessorType).FromJust()) {
      v8::Local<v8::Value> value =
          descriptor->Get(context, accessorType).ToLocalChecked();
      if (value->IsFunction()) {
        nativeFn = v8::Local<v8::Function>::Cast(value);
      } else {
        return nativeFn;  // Returns an undefined function
      }
    }

    if (config->HasOwnProperty(context, accessorType).FromJust()) {
      v8::Local<v8::Object> callbackData = v8::Object::New(isolate);
      // if (!nativeFn.IsEmpty()) {
      //     (void)callbackData->Set(context, v8::String::NewFromUtf8(isolate,
      //     "super").ToLocalChecked(), nativeFn);
      // }

      (void)callbackData->Set(
          context, v8::String::NewFromUtf8(isolate, "handler").ToLocalChecked(),
          v8::Local<v8::Function>::Cast(
              config->Get(context, accessorType).ToLocalChecked()));
      (void)callbackData->Set(
          context, v8::String::NewFromUtf8(isolate, "target").ToLocalChecked(),
          nativeFn);

      v8::Local<v8::FunctionTemplate> templateFn =
          v8::FunctionTemplate::New(isolate, patchWrapper, callbackData);
      templateFn->RemovePrototype();
      templateFn->SetClassName(propName);

      v8::Local<v8::Function> fnWrapped =
          templateFn->GetFunction(context).ToLocalChecked();

      CopyOwnPropertiesDescriptors(nativeFn, fnWrapped, context, isolate);

      return fnWrapped;
    }
    return nativeFn;
  };

  if (!descriptor->Has(context, __GET__).FromJust() &&
      !descriptor->Has(context, __SET__).FromJust()) {
    ThrowAndLogError(isolate,
                     "Cannot patch a data descriptor with this PatchAccessor. "
                     "Use PatchValue instead.");
    return;
  }

  v8::Local<v8::Function> patchedGetterFn = ProcessAccessor(__GET__);
  v8::Local<v8::Function> patchedSetterFn = ProcessAccessor(__SET__);

  v8::PropertyAttribute srcPropAttrs =
      holder->GetPropertyAttributes(context, propName).FromJust();

  v8::PropertyDescriptor newDescriptor(patchedGetterFn, patchedSetterFn);

  newDescriptor.set_enumerable((srcPropAttrs & v8::PropertyAttribute::DontEnum) ==
                               0);
  newDescriptor.set_configurable(
      (srcPropAttrs & v8::PropertyAttribute::DontDelete) == 0);

  // Will always succeeed because of Patched
  (void)holder->DefineProperty(context, propName, newDescriptor);
}

void PatchValue(const v8::FunctionCallbackInfo<v8::Value>& args) {
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
  v8::Isolate* isolate = args.GetIsolate();
  v8::HandleScope handle_scope(isolate);
  v8::Local<v8::Context> context = isolate->GetCurrentContext();

  v8::Local<v8::String> __VALUE__ =
      v8::String::NewFromUtf8(isolate, "value").ToLocalChecked();

  // Check for the correct number of arguments
  if (args.Length() != 3) {
    ThrowAndLogError(isolate,
                     "Expected 3 arguments: target, property name, and value.");
    return;
  }

  // Validation for object and string arguments
  auto ValidateArg = [&](int index, bool should_be_object,
                         const char* message) -> v8::Local<v8::Value> {
    if ((should_be_object && !args[index]->IsObject()) ||
        (!should_be_object && !args[index]->IsString())) {
      ThrowAndLogError(isolate, message);
      return {};
    }
    return args[index];
  };

  v8::Local<v8::Object> holder = v8::Local<v8::Object>::Cast(ValidateArg(
      0, true, "Expected first argument to be an object (holder)."));
  v8::Local<v8::String> propName = v8::Local<v8::String>::Cast(ValidateArg(
      1, false, "Expected second argument to be a string (property name)."));
  v8::Local<v8::Value> handler = args[2];

  v8::MaybeLocal<v8::Value> maybeDescriptor =
      holder->GetOwnPropertyDescriptor(context, propName);

  if (maybeDescriptor.IsEmpty()) {
    ThrowAndLogError(isolate, "Property does not exist on target object.");
    return;
  }

  v8::Local<v8::Object> descriptor =
      v8::Local<v8::Object>::Cast(maybeDescriptor.ToLocalChecked());

  // Check that the descriptor is a data descriptor. Must have a .value

  if (!descriptor->Has(context, __VALUE__).FromJust()) {
    ThrowAndLogError(isolate,
                     "Cannot patch an accessor descriptor with this "
                     "PatchValue. Use PatchAccessor instead.");
    return;
  }

  // The target object being patched (the property of the holder)
  v8::Local<v8::Value> target =
      descriptor->Get(context, __VALUE__).ToLocalChecked();

  if (!handler->IsFunction()) {
    ThrowAndLogError(isolate, "Expected third argument to be a function.");
    return;
  }

  v8::Local<v8::Object> wrapperInnerArgs = v8::Object::New(isolate);

  (void)wrapperInnerArgs
      ->Set(context,
            v8::String::NewFromUtf8(isolate, "handler").ToLocalChecked(),
            handler)
      .ToChecked();
  (void)wrapperInnerArgs
      ->Set(context,
            v8::String::NewFromUtf8(isolate, "holder").ToLocalChecked(), holder)
      .ToChecked();
  (void)wrapperInnerArgs
      ->Set(context,
            v8::String::NewFromUtf8(isolate, "target").ToLocalChecked(), target)
      .ToChecked();

  v8::Local<v8::Object> oTarget = v8::Local<v8::Object>::Cast(target);

  v8::Local<v8::FunctionTemplate> templateFn =
      v8::FunctionTemplate::New(isolate, patchWrapper, wrapperInnerArgs);

  templateFn->RemovePrototype();
  templateFn->SetClassName(propName);

  v8::Local<v8::Function> fnWrapped =
      templateFn->GetFunction(context).ToLocalChecked();

  CopyOwnPropertiesDescriptors(oTarget, fnWrapped, context, isolate);

  v8::PropertyAttribute targetPropertyAttributes =
      holder->GetPropertyAttributes(context, propName).FromJust();

  // Should always evaluate to true because of Patched
  (void)holder->DefineOwnProperty(
      context, propName, fnWrapped,
      (v8::PropertyAttribute)(targetPropertyAttributes |
                          v8::PropertyAttribute::Patched));
}

}  // namespace utilities
}  // namespace fingerprinting
