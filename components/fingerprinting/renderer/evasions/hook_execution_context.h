
#ifndef FINGERPRINTING_PUBLIC_EVASIONS_HOOK_EXECUTION_CONTEXT_H_
#define FINGERPRINTING_PUBLIC_EVASIONS_HOOK_EXECUTION_CONTEXT_H_

#include "base/component_export.h"
#include "components/fingerprinting/renderer/evasions/hook.h"
#include "v8/include/v8.h"

namespace blink {
class ExecutionContext;
}
namespace fingerprinting::evasions {

class EvasionsPackageExecutionContext;

using core::evasions::HookTargetType;
using fingerprinting::core::evasions::Hook;
class HookExecutionContext {
 public:
  explicit HookExecutionContext(EvasionsPackageExecutionContext* package,
                                Hook* hook);

  v8::MaybeLocal<v8::Function> GetFunction(v8::Local<v8::Context>);

  // Invocation data
  // v8::Local<v8::Value> GetArguments();

  bool Run(v8::Local<v8::Value>* args = nullptr);

  static HookTargetType TypeFromExecutionContext(blink::ExecutionContext*);

  bool operator<(HookExecutionContext& other) {
    if (hook_->priority() == other.hook_->priority()) {
      return hook_ > other.hook_;  // Tie-breaker if priorities are Equality
    }
    return hook_->priority() > other.hook_->priority();
  }

  // Equality operator
  friend struct HookExecutionContextPtrComparator;

  auto& Hook() { return hook_; }

 private:
  EvasionsPackageExecutionContext* package_;
  fingerprinting::core::evasions::Hook* hook_;
};
struct HookExecutionContextPtrComparator {
  bool operator()(const std::unique_ptr<HookExecutionContext>& lhs,
                  const std::unique_ptr<HookExecutionContext>& rhs) const {
    if (lhs->hook_->priority() == rhs->hook_->priority()) {
      return lhs->Hook()->codename() <
             rhs->Hook()->codename();  // Tie-breaker if priorities are Equality
    }
    return lhs->hook_->priority() >
           rhs->hook_->priority();  // Assuming Hook has an operator< defined
  }

  // overload for rw pointers
};
}  // namespace fingerprinting::evasions
#endif  // FINGERPRINTING_PUBLIC_EVASIONS_HOOK_EXECUTION_CONTEXT_H_
