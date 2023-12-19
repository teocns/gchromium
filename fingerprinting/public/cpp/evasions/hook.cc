

#include "fingerprinting/public/cpp/evasions/hook.h"
#include "v8/include/v8.h"
namespace fingerprinting::evasions {

namespace {
class Package;
}

v8::MaybeLocal<v8::Function> Hook::GetFunction(v8::Local<v8::Context> context) {
  v8::Isolate* isolate = context->GetIsolate();

  // Create an IIFE
  v8::Local<v8::String> js_src;
  if (!v8::String::NewFromUtf8(
           isolate, std::format("(function (){{ {} }})", get_impl()).c_str(),
           v8::NewStringType::kNormal)
           .ToLocal(&js_src)) {
    // isolate->ThrowException(v8::Exception::Error(
    //     v8::String::NewFromUtf8(isolate, "Failed to create JS source
    //     string").ToLocalChecked()));
    return v8::MaybeLocal<v8::Function>();
  }

  v8::Local<v8::Script> script;
  if (!v8::Script::Compile(context, js_src).ToLocal(&script)) {
    // isolate->ThrowException(v8::Exception::Error(
    //     v8::String::NewFromUtf8(isolate, "Failed to compile
    //     script").ToLocalChecked()));
    return v8::MaybeLocal<v8::Function>();
  }

  v8::Local<v8::Value> maybeFunction;
  if (!script->Run(context).ToLocal(&maybeFunction)) {
    // isolate->ThrowException(v8::Exception::Error(
    //     v8::String::NewFromUtf8(isolate, "Failed to run
    //     script").ToLocalChecked()));
    return v8::MaybeLocal<v8::Function>();
  }

  return v8::Local<v8::Function>::Cast(maybeFunction);
}

}  // namespace fingerprinting::evasions
