#ifndef FINGERPRINTING_PUBLIC_EVASIONS_PACKAGE_EXECUTION_CONTEXT_H_
#define FINGERPRINTING_PUBLIC_EVASIONS_PACKAGE_EXECUTION_CONTEXT_H_

#include <memory>
#include <string>
#include <unordered_map>
#include "base/component_export.h"
#include "fingerprinting/core/device_descriptor/fingerprint_impl.h"
#include "components/fingerprinting/renderer/evasions/pack.h"
#include "components/fingerprinting/renderer/evasions/hook_execution_context.h"
#include "third_party/blink/renderer/platform/bindings/script_state.h"
#include "v8/include/v8.h"

namespace fingerprinting::evasions {

class EvasionsPackageExecutionContext {
  // Takes an EvasionsPack and runs it into a V8 context

 public:
  explicit EvasionsPackageExecutionContext(
      core::evasions::EvasionsPackage*,
      // The reason we pass the ScriptState* and not the ExecutionContext* is
      // because the former provides a way to get the latter and not viceversa
      blink::ScriptState*,
      fingerprinting::Fingerprint*);

  v8::Local<v8::Object> GetCommonArguments();

  /**
   *
   * Runs the whole package into a V8 Context
   */
  void Run();

  friend class HookExecutionContext;
 private:
  // A map whereas the key is the hook object (not the string) and the value is
  // the execution context

  // std::unordered_map<std::string, std::unique_ptr<HookExecutionContext>>
  // hooks_;

  std::unordered_map<std::string, std::unique_ptr<HookExecutionContext>> hooks_;
  blink::ScriptState* script_state_;
  fingerprinting::Fingerprint* fingerprint_;
};

}  // namespace fingerprinting::evasions

#endif  // FINGERPRINTING_PUBLIC_EVASIONS_PACKAGE_EXECUTION_CONTEXT_H_
