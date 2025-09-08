

#include "fingerprinting/public/evasions/hook_execution_context.h"
#include <format>
#include "base/logging.h"
#include "fingerprinting/public/evasions/package_execution_context.h"
#include "v8-local-handle.h"
#include "v8-object.h"
namespace fingerprinting::evasions {

v8::Local<v8::Value> HookExecutionContext::GetArguments() {
  // A value array of arguments
  return this->package_->GetCommonArguments();
}

bool HookExecutionContext::Run() {
  v8::Local<v8::Context> context = this->package_->context();

  v8::Isolate* isolate = context->GetIsolate();

  v8::TryCatch try_catch(isolate);

  v8::MaybeLocal<v8::Function> fn = GetFunction(context);

  if (fn.IsEmpty()) {
    return false;
  }

  v8::Local<v8::Function> function = fn.ToLocalChecked();

  v8::Local<v8::Value> args[1] = {GetArguments()};

  v8::MaybeLocal<v8::Value> result =
      function->Call(context, context->Global(), 1, args);

  if (try_catch.HasCaught()) {
    v8::Local<v8::Value> exception = try_catch.Exception();
    v8::String::Utf8Value exception_str(isolate, exception);
    LOG(ERROR) << "Exception: " << *exception_str;
    return false;
  }
  LOG(INFO) << "Hook " << this->hook_->codename() << " executed successfully";
  LOG(INFO) << "Result: "
            << *v8::String::Utf8Value(isolate, result.ToLocalChecked());
  return true;
}

v8::MaybeLocal<v8::Function> HookExecutionContext::GetFunction(
    v8::Local<v8::Context> context) {
  v8::Isolate* isolate = context->GetIsolate();

  // Create an IIFE
  v8::Local<v8::String> js_src;
  if (!v8::String::NewFromUtf8(
           isolate,
           std::format("(function (){{ {} }})", hook_->get_impl()).c_str(),
           v8::NewStringType::kNormal)
           .ToLocal(&js_src)) {
    return v8::MaybeLocal<v8::Function>();
  }

  v8::Local<v8::Script> script;
  if (!v8::Script::Compile(context, js_src).ToLocal(&script)) {
    return v8::MaybeLocal<v8::Function>();
  }

  v8::Local<v8::Value> maybeFunction;
  if (!script->Run(context).ToLocal(&maybeFunction)) {
    return v8::MaybeLocal<v8::Function>();
  }

  return v8::Local<v8::Function>::Cast(maybeFunction);
}

HookExecutionContext::HookExecutionContext(
    EvasionsPackageExecutionContext* package,
    std::unique_ptr<fingerprinting::core::evasions::Hook> hook)
    : package_(package), hook_(std::move(hook)) {}
}  // namespace fingerprinting::evasions
