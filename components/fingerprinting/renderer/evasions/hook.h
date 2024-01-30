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
  bool operator<(Hook& other) { return priority() < other.priority(); }

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

  friend class HookFactory;

 private:
  std::string impl;
};

class HookIterator {
 public:
  explicit HookIterator(std::vector<std::unique_ptr<Hook>>& foos)
      : it(foos.begin()), end(foos.end()) {}

  bool hasNext() const { return it != end; }

  Hook* next() { return it++->get(); }

  // Comparator for sorting unique_ptr<Hook> by priority
  bool cmp(const std::unique_ptr<Hook>& a, const std::unique_ptr<Hook>& b) {
    return a->priority() > b->priority();
  }

  virtual HookTargetType AllowdTargetTypes() { return HookTargetType::ALL; }

 private:
  std::vector<std::unique_ptr<Hook>>::iterator it, end;
};

}  // namespace fingerprinting::core::evasions

#endif  // COMPONENTS_FINGERPRINTING_EVASIONS_HOOK_H_
