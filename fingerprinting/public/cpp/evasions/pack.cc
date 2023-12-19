
#include "fingerprinting/public/cpp/evasions/pack.h"

#include "fingerprinting/utility/v8.h"
#include "v8/include/v8-function.h"
#include "v8/include/v8-microtask-queue.h"
#include "base/logging.h"
namespace fingerprinting::evasions {

void ::fingerprinting::evasions::Package::Inject(
    v8::Local<v8::Context> context) {
  v8::Isolate* isolate = context->GetIsolate();

  // Begin a new scope for handles.
  v8::HandleScope handle_scope(isolate);

  // Enter context with a new microtasks scope if you want to run microtasks
  // after the script execution.
  v8::MicrotasksScope microtasks(isolate, v8::MicrotasksScope::kRunMicrotasks);
  v8::Context::Scope context_scope(context);

  for (auto hook : hooks) {
    v8::MaybeLocal<v8::Function> function = hook.GetFunction(context);
    if (function.IsEmpty()) {
      LOG(ERROR) << "Failed to get function for hook " << hook.codename;
      continue;
    }
    v8::Local<v8::Function> local_function = function.ToLocalChecked();

    v8::Local<v8::Value> result;

    if (!local_function->Call(context, context->Global(), 0, nullptr)
             .ToLocal(&result)) {
      LOG(ERROR) << "Ran function for hook " << hook.codename
                 << " but it failed";
    }
  }
}

}  // namespace fingerprinting::evasions
