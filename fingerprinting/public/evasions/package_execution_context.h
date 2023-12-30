#ifndef FINGERPRINTING_PUBLIC_EVASIONS_PACKAGE_EXECUTION_CONTEXT_H_
#define FINGERPRINTING_PUBLIC_EVASIONS_PACKAGE_EXECUTION_CONTEXT_H_

#include <memory>
#include <string>
#include "base/component_export.h"
#include "fingerprinting/core/device_descriptor/fingerprint_impl.h"
#include "fingerprinting/core/evasions/pack.h"
#include "fingerprinting/public/cpp/evasions/hook_execution_context.h"
#include "v8/include/v8.h"
#include "content/renderer/v8_value_converter_impl.h"

namespace fingerprinting::evasions {

class COMPONENT_EXPORT(FINGERPRINTING_PUBLIC_EVASIONS)
    EvasionsPackageExecutionContext {
  // Takes an EvasionsPack and runs it into a V8 context

 public:
  explicit EvasionsPackageExecutionContext(
      std::unique_ptr<::fingerprinting::core::evasions::EvasionsPackage>,
      v8::Local<v8::Context>,
      std::unique_ptr<::fingerprinting::Fingerprint>);

  v8::Local<v8::Object> GetCommonArguments();

  /**
   *
   * Runs the whole package into a V8 Context
   */
  void Run();
  // bool RunHook(std::unique_ptr<::fingerprinting::core::evasions::Hook> hook);

  v8::Local<v8::Context> context() { return context_; }

 private:
  // A map whereas the key is the hook object (not the string) and the value is
  // the execution context

  std::unordered_map<std::string, std::unique_ptr<HookExecutionContext>> hooks_;

  v8::Local<v8::Object> state_;
  v8::Local<v8::Context> context_;
  std::unique_ptr<::fingerprinting::Fingerprint> fingerprint_;
};

}  // namespace fingerprinting::evasions

#endif  // FINGERPRINTING_PUBLIC_EVASIONS_PACKAGE_EXECUTION_CONTEXT_H_
