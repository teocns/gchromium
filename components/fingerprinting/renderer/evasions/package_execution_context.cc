#include "components/fingerprinting/renderer/evasions/package_execution_context.h"
#include "base/logging.h"
#include "base/time/time.h"
#include "components/fingerprinting/renderer/evasions/pack.h"
#include "components/fingerprinting/renderer/helper/v8.h"
#include "third_party/blink/renderer/platform/bindings/script_state.h"
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
    std::string codename = hook->codename();
    this->hooks_[codename] =
        std::make_unique<HookExecutionContext>(this, std::move(hook));
  }
}
void EvasionsPackageExecutionContext::Run() {
  // Iterate over the map of this->hooks_ [string, HookExecutionContext]
  for (auto& [_, hook] : this->hooks_) {
    // Run the hook
    hook->Run();
  }
}

v8::Local<v8::Object> EvasionsPackageExecutionContext::GetCommonArguments() {
  // Wrap fingerprint in a v8 external, but let's track the timing needed for
  // this

  v8::Isolate* isolate = this->script_state_->GetIsolate();
  base::TimeTicks start = base::TimeTicks::Now();

  v8::Local<v8::External> wrapped_fingerprint =
      v8::External::New(isolate, &this->fingerprint_->str_value());

  base::TimeTicks end = base::TimeTicks::Now();
  base::TimeDelta delta = end - start;
  DLOG(INFO) << "It took " << delta.InMilliseconds()
             << "ms to wrap the fingerprint in a v8 external";

  v8::Local<v8::Object> obj = v8::Object::New(isolate);

  v8::Local<v8::Context> context = isolate->GetCurrentContext();

  (void)obj
      ->Set(context, v8::String::NewFromUtf8(isolate, "dd").ToLocalChecked(),
            wrapped_fingerprint)
      .ToChecked();

  // ---------------
  v8::Local<v8::Function> f1 =
      v8::FunctionTemplate::New(
          isolate, ::fingerprinting::utility::PatchAccessor)
          ->GetFunction(context)
          .ToLocalChecked();
  v8::Local<v8::Function> f2 =
      v8::FunctionTemplate::New(isolate,
                                ::fingerprinting::utility::PatchValue)
          ->GetFunction(context)
          .ToLocalChecked();

  bool p1 =
      obj->Set(
             context,
             v8::String::NewFromUtf8(isolate, "PatchAccessor").ToLocalChecked(),
             f1)
          .ToChecked();
  bool p2 =
      obj->Set(context,
               v8::String::NewFromUtf8(isolate, "PatchValue").ToLocalChecked(),
               f2)
          .ToChecked();

  if (!p1 || !p2) {
    LOG(ERROR) << "Failed to set PatchAccessor or PatchValue";
  }

  bool p3 =
      obj->Set(context, v8::String::NewFromUtf8(isolate, "dd").ToLocalChecked(),
               v8::External::New(isolate, this->fingerprint_))
          .ToChecked();

  if (!p3) {
    LOG(ERROR) << "Failed to set dd";
  }

  return obj;

  //
  //   return this->state_;
}  // namespace fingerprinting::evasions

}  // namespace fingerprinting::evasions
