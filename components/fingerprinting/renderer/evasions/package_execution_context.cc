#include "base/logging.h"
#include "components/fingerprinting/renderer/evasions/package_execution_context.h"
#include "components/fingerprinting/renderer/evasions/pack.h"
#include "third_party/blink/renderer/platform/bindings/script_state.h"
namespace fingerprinting::evasions {

void EvasionsPackageExecutionContext::Run() {
  // Createa a context that will be shared among all the hooks
  //
  //   v8::Isolate* isolate = this->context()->GetIsolate();
  //
  //   // Begin a new scope for handles.
  //   v8::HandleScope handle_scope(isolate);
  //
  //   // Enter context with a new microtasks scope if you want to run
  //   microtasks
  //   // after the script execution.
  //   v8::MicrotasksScope microtasks(isolate,
  //   v8::MicrotasksScope::kRunMicrotasks);
  //
  //   v8::Context::Scope context_scope(this->context());
  //
  //   // Iterate over the map of this->hooks_ [string, HookExecutionContext]
  //   for (auto& [_, hook] : this->hooks_) {
  //     // Run the hook
  //     hook->Run();
  //   }
  // }
  //
  // v8::Local<v8::Object> EvasionsPackageExecutionContext::GetCommonArguments()
  // {
  //   // Gets common arguments for the current execution context
  //   // All hooks must have access to these arguments
  //
  //   v8::Isolate* isolate = this->context()->GetIsolate();
  //
  //   if (this->state_.IsEmpty()) {
  //     // Create a new object (Dictionary)
  //     this->state_ = v8::Object::New(isolate);
  //
  //     // Add the function utilities
  //
  //     v8::Local<v8::Function> f1 =
  //         v8::FunctionTemplate::New(
  //             isolate, ::fingerprinting::evasions::utility::PatchAccessor)
  //             ->GetFunction(this->context())
  //             .ToLocalChecked();
  //
  //     v8::Local<v8::Function> f2 =
  //         v8::FunctionTemplate::New(
  //             isolate, ::fingerprinting::evasions::utility::PatchValue)
  //             ->GetFunction(this->context())
  //             .ToLocalChecked();
  //
  //     bool p1 = this->state_
  //                   ->Set(this->context(),
  //                         v8::String::NewFromUtf8(isolate, "PatchAccessor")
  //                             .ToLocalChecked(),
  //                         f1)
  //                   .ToChecked();
  //
  //     bool p2 =
  //         this->state_
  //             ->Set(
  //                 this->context(),
  //                 v8::String::NewFromUtf8(isolate,
  //                 "PatchValue").ToLocalChecked(), f2)
  //             .ToChecked();
  //
  //     if (!p1 || !p2) {
  //       LOG(ERROR) << "Failed to set PatchAccessor or PatchValue";
  //     }
  //
  //     // Set dd. Fingerprint::value() returns a base::Value* object
  //     bool p3 = this->state_
  //                   ->Set(this->context(),
  //                         v8::String::NewFromUtf8(isolate,
  //                         "dd").ToLocalChecked(), v8::External::New(isolate,
  //                         this->fingerprint_))
  //                   .ToChecked();
  //
  //     if (!p3) {
  //       LOG(ERROR) << "Failed to set dd";
  //     }
  //   }
  // Create a one-off fingerprint object

  // return this->state_;
}

EvasionsPackageExecutionContext::EvasionsPackageExecutionContext(
    core::evasions::EvasionsPackage* package,
    blink::ScriptState* scirpt_state,
    fingerprinting::Fingerprint* fingerprinting) {
  // Each hook gets its own execution context
  // for (auto& hook : package->hooks) {
  //   std::string codename = hook->codename();
  //   this->hooks_[codename] =
  //       std::make_unique<HookExecutionContext>(this, std::move(hook));
  // }
}
}  // namespace fingerprinting::evasions
