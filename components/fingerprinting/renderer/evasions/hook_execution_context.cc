#include "components/fingerprinting/renderer/evasions/hook_execution_context.h"
#include <format>
#include "base/debug/stack_trace.h"
#include "base/logging.h"
#include "components/fingerprinting/renderer/evasions/package_execution_context.h"
#include "v8-local-handle.h"
#include "v8-object.h"
#include "v8-script.h"
namespace fingerprinting::evasions {

v8::Local<v8::Value> HookExecutionContext::GetArguments() {
  // A value array of arguments
  return this->package_->GetCommonArguments();
}

bool HookExecutionContext::Run() {
  v8::Local<v8::Context> context = this->package_->script_state_->GetContext();
  v8::Isolate* isolate = context->GetIsolate();
  //

  // Begin a new scope for handles.
  //
  // // Enter context with a new microtasks scope if you want to run microtasks
  // // after the script execution.
  //
  v8::Context::Scope context_scope(context);

  v8::MicrotasksScope microtasks(isolate, context->GetMicrotaskQueue(),
                                 v8::MicrotasksScope::kRunMicrotasks);

  v8::HandleScope handle_scope(isolate);
  //
  v8::TryCatch try_catch(isolate);
  //
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
    LOG(ERROR) << "Exception [" << this->package_->Kind() << "]:" << *exception_str;
    // Print stacktrace of this thread
    // base::debug::StackTrace().Print();

    return false;
  }
  LOG(INFO) << "[" << this->package_->Kind() << "]:" << this->hook_->codename() << " hook OK";
  LOG(INFO) << "Result: "
            << *v8::String::Utf8Value(isolate, result.ToLocalChecked());
  return true;
}

v8::MaybeLocal<v8::Function> HookExecutionContext::GetFunction(
    v8::Local<v8::Context> context) {
  v8::Isolate* isolate = context->GetIsolate();

  v8::EscapableHandleScope handle_scope(isolate);
  // Create an IIFE
  v8::Local<v8::String> js_src;
  if (!v8::String::NewFromUtf8(
           isolate,
           std::format("(function (){{ {} }})", hook_->get_impl()).c_str(),
           v8::NewStringType::kNormal)
           .ToLocal(&js_src)) {
    return v8::MaybeLocal<v8::Function>();
  }
  v8::MaybeLocal<v8::UnboundScript> compiled_src;
  v8::ScriptCompiler::Source script_source(js_src);

  v8::MicrotasksScope microtasks(isolate, context->GetMicrotaskQueue(),
                                 v8::MicrotasksScope::kRunMicrotasks);

  compiled_src =
      v8::ScriptCompiler::CompileUnboundScript(isolate, &script_source);
  if (compiled_src.IsEmpty()) {
    LOG(ERROR) << "Failed to compile hook script";
    return v8::MaybeLocal<v8::Function>();
  }

  v8::Local<v8::Value> maybeFunction;

  // Bind the script to the context so that we can run it

  auto runnable = compiled_src.ToLocalChecked()->BindToCurrentContext();
  // isolate->PerformMicrotaskCheckpoint();
  if (!runnable->Run(context).ToLocal(&maybeFunction)) {
    return v8::MaybeLocal<v8::Function>();
  }

  return handle_scope.Escape(v8::Local<v8::Function>::Cast(maybeFunction));
}

// v8::Local<v8::Value> HookExecutionContext::RunScript(
//     v8::Local<v8::String> name,
//     v8::Local<v8::String> code) {
//
//   v8::Isolate* isolate = context->GetIsolate();
//   v8::Local<v8::Context> context = isolate->GetCurrentContext();
//   v8::EscapableHandleScope handle_scope(isolate);
//   v8::Context::Scope context_scope(context);
//
//   // Prepend extensions:: to |name| so that internal code can be
//   differentiated
//   // from external code in stack traces. This has no effect on behaviour.
//   std::string internal_name = base::StringPrintf(
//       "extensions::%s", *v8::String::Utf8Value(isolate(), name));
//
//   if (internal_name.size() >= v8::String::kMaxLength) {
//     NOTREACHED() << "internal_name is too long.";
//     return v8::Undefined(isolate());
//   }
//
//   v8::MicrotasksScope microtasks(isolate(),
//   v8_context()->GetMicrotaskQueue(),
//                                  v8::MicrotasksScope::kDoNotRunMicrotasks);
//   v8::TryCatch try_catch(isolate());
//   try_catch.SetCaptureMessage(true);
//   v8::ScriptOrigin origin(isolate(), v8_helpers::ToV8StringUnsafe(
//                                          isolate(), internal_name.c_str()));
//   v8::ScriptCompiler::Source script_source(code, origin);
//   v8::Local<v8::Script> script;
//   if (!v8::ScriptCompiler::Compile(v8_context(), &script_source,
//                                    v8::ScriptCompiler::kNoCompileOptions,
//                                    no_cache_reason)
//            .ToLocal(&script)) {
//     std::move(exception_handler).Run(try_catch);
//     return v8::Undefined(isolate());
//   }
//
//   v8::Local<v8::Value> result;
//   if (!script->Run(v8_context()).ToLocal(&result)) {
//     std::move(exception_handler).Run(try_catch);
//     return v8::Undefined(isolate());
//   }
//
//   return handle_scope.Escape(result);
// }
HookExecutionContext::HookExecutionContext(
    EvasionsPackageExecutionContext* package,
    std::unique_ptr<fingerprinting::core::evasions::Hook> hook)
    : package_(package), hook_(std::move(hook)) {}
}  // namespace fingerprinting::evasions
