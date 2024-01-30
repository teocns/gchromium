#ifndef COMPONENTS_FINGERPRINTING_EVASIONS_HOOK_H_
#define COMPONENTS_FINGERPRINTING_EVASIONS_HOOK_H_

#include <format>
#include <functional>
#include <map>
#include <random>
#include <string>
#include "base/component_export.h"
#include "base/values.h"
#include "fingerprinting/core/device_descriptor/fingerprint_impl.h"

namespace {
class EvasionsPackage;
}

namespace fingerprinting::core::evasions {

enum class HookTargetType : unsigned int {
  NONE = 0,                              // No target, 0b000
  WINDOW = 1 << 0,                       // Window target, 0b001
  WORKER = (1 << 1) | (1 << 2),          // Worker target, 0b110
  SHARED_WORKER = 1 << 2,                // SharedWorker target, 0b100
  ALL = WINDOW | WORKER | SHARED_WORKER  // All targets, 0b111
};

inline HookTargetType operator&(HookTargetType a, HookTargetType b) {
  return static_cast<HookTargetType>(static_cast<unsigned int>(a) &
                                     static_cast<unsigned int>(b));
}
class Hook {
  /*
   * Each hook represents a patch for a specific JS feature API
   * (e.g. WebGL, Canvas, etc.) A hook instance binds with a
   * specific target (e.g. Worker, Window, etc.)
   */

 public:
  Hook() = default;
  Hook(const Hook& other);
  Hook(Hook&& other) = default;
  Hook& operator=(const Hook& other) = default;
  Hook& operator=(Hook&& other) = default;

  // Priority queues only need the LLT operator
  bool operator<(Hook& other) {
    if (priority() == other.priority()) {
      return codename() >
             other.codename();  // Tie-breaker if priorities are equal
    }
    return priority() > other.priority();
  }

  virtual ~Hook() = default;

  // Returns immediately-invoked function expression (IIFE)
  std::string get_iife();

  virtual std::string codename() = 0;
  /*
   * The implementation [body inner part] of the the hook function definition
   * (aka routine) This is where the actual patch logic is defined by
   * implementors of `Hook` (subclasses)
   */
  virtual std::string get_impl() = 0;

  /*
   * Implementors define a getter for the data required to be accessed by the
   * hook
   */
  virtual base::Value* get_data(Fingerprint* fingerprint) { return nullptr; }

  // The priority of the hook
  // The higher the priority, the earlier the hook will be executed
  static int priority() { return 0; }

  std::shared_ptr<EvasionsPackage> package = nullptr;
  virtual HookTargetType AllowedTargetTypes() { return HookTargetType::ALL; }
  friend class HookFactory;

 private:
  std::string impl;
};

struct HookPtrComparator {
  bool operator()(const std::unique_ptr<Hook>& lhs,
                  const std::unique_ptr<Hook>& rhs) const {
    // Compare the Hook objects pointed to by lhs and rhs
    // Make sure to check for null if that's a possibility
    if (lhs->priority() == rhs->priority()) {
      return lhs.get() < rhs.get();  // Tie-breaker if priorities are Equality
    }
    return lhs->priority() <
           rhs->priority();  // Assuming Hook has an operator< defined
  }

  // overload for rw pointers
};
}  // namespace fingerprinting::core::evasions

#endif  // COMPONENTS_FINGERPRINTING_EVASIONS_HOOK_H_
