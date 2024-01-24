
#ifndef FINGERPRINTING_PUBLIC_EVASIONS_HOOK_EXECUTION_CONTEXT_H_
#define FINGERPRINTING_PUBLIC_EVASIONS_HOOK_EXECUTION_CONTEXT_H_

#include "base/component_export.h"
#include "fingerprinting/core/evasions/hook.h"
#include "v8/include/v8.h"

namespace fingerprinting::evasions {

class EvasionsPackageExecutionContext;

class  HookExecutionContext {
 public:
  explicit HookExecutionContext(
      EvasionsPackageExecutionContext* package,
      std::unique_ptr<fingerprinting::core::evasions::Hook> hook);

  v8::MaybeLocal<v8::Function> GetFunction(v8::Local<v8::Context>);

  // Invocation data
  v8::Local<v8::Value> GetArguments();

  bool Run();

 private:
  EvasionsPackageExecutionContext* package_;
  std::unique_ptr<fingerprinting::core::evasions::Hook> hook_;
};

}  // namespace fingerprinting::evasions
#endif  // FINGERPRINTING_PUBLIC_EVASIONS_HOOK_EXECUTION_CONTEXT_H_
