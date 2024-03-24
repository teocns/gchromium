#include "components/fingerprinting/renderer/evasions/package_execution_context.h"
#include "base/logging.h"
#include "base/time/time.h"
#include "components/fingerprinting/renderer/evasions/pack.h"
#include "components/fingerprinting/renderer/helper/v8.h"
#include "third_party/blink/renderer/platform/bindings/script_state.h"
#include "third_party/blink/renderer/platform/bindings/v8_binding.h"
#include "v8/include/v8.h"
namespace fingerprinting::evasions {
EvasionsPackageExecutionContext::EvasionsPackageExecutionContext(
    core::evasions::EvasionsPackage* package,
    blink::ScriptState* scirpt_state,
    fingerprinting::Fingerprint* fingerprinting) {
  this->script_state_ = scirpt_state;
  this->fingerprint_ = fingerprinting;
  // Each hook gets its own execution context
  for (auto& hook : package->hooks) {
    this->hooks_.insert(
        std::make_unique<HookExecutionContext>(this, hook.get()));
  }
}
void EvasionsPackageExecutionContext::Run() {
  // Iterate over the map of this->hooks_ [string, HookExecutionContext]
  v8::Local<v8::Context> context = this->script_state_->GetContext();
  v8::Isolate* isolate = context->GetIsolate();
  v8::Context::Scope context_scope(context);
  v8::HandleScope handle_scope(isolate);
  v8::Local<v8::Value> args;

  {
    v8::MicrotasksScope microtasks(isolate, context->GetMicrotaskQueue(),
                                   v8::MicrotasksScope::kRunMicrotasks);
    args = this->GetCommonArguments();
  }
  for (auto& hook : this->hooks_) {
    // Run the hook
    hook->Run(&args);
  }
}

v8::Local<v8::Object> EvasionsPackageExecutionContext::GetCommonArguments() {
  // Returns the arguments passed to each Hook's invocation
  // Namingly:
  // - dd: the device descriptor
  // - PatchAccessor: a function that patches an accessor
  // - PatchValue: a function that patches a Value
  // - results[codename]: the result of each hook

  v8::Local<v8::Context> context = this->script_state_->GetContext();
  v8::Isolate* isolate = context->GetIsolate();

  v8::Local<v8::Object> obj = v8::Object::New(isolate);

  // ---------------
  v8::Local<v8::Function> f1 =
      v8::FunctionTemplate::New(isolate,
                                ::fingerprinting::utility::PatchAccessor)
          ->GetFunction(context)
          .ToLocalChecked();

  v8::Local<v8::Function> f2 =
      v8::FunctionTemplate::New(isolate, ::fingerprinting::utility::PatchValue)
          ->GetFunction(context)
          .ToLocalChecked();

  (void)obj->Set(context, blink::V8String(isolate, "PatchAccessor"), f1)
      .ToChecked();

  (void)obj->Set(context, blink::V8String(isolate, "PatchValue"), f2)
      .ToChecked();

  // Compile the JSON string into a v8::Value
  base::TimeTicks start = base::TimeTicks::Now();

  // Attempt retrieval of v8::Value from the persistent value

  v8::ScriptCompiler::Source source(
      v8::String::NewFromUtf8(
          isolate, ("(" + this->fingerprint_->str_value() + ")").c_str())
          .ToLocalChecked());

  v8::MaybeLocal<v8::UnboundScript> fp_maybe_script =
      v8::ScriptCompiler::CompileUnboundScript(isolate, &source);

  if (fp_maybe_script.IsEmpty()) {
    LOG(ERROR) << "Could not compute fingerprint string JSON into a JS object";
    return obj;
  }

  auto runnable = fp_maybe_script.ToLocalChecked()->BindToCurrentContext();

  v8::Local<v8::Value> fp_val;
  // isolate->PerformMicrotaskCheckpoint();
  if (!runnable->Run(context).ToLocal(&fp_val)) {
    LOG(ERROR)
        << "[RUN] Could not compute fingerprint string JSON into a JS object";
    return obj;
  }

  (void)obj->Set(context, blink::V8String(isolate, "dd"), fp_val).ToChecked();

  // Create a dictionary for results
  (void)obj
      ->Set(context, blink::V8String(isolate, "results"),
            v8::Object::New(isolate))
      .ToChecked();

  base::TimeTicks end = base::TimeTicks::Now();
  base::TimeDelta delta = end - start;
  DLOG(INFO) << "It took " << delta.InMilliseconds()
             << "ms to set the fingerprint string";
  return obj;
}

}  // namespace fingerprinting::evasions
