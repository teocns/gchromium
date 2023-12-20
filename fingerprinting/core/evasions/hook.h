#ifndef FINGERPRINTING_EVASIONS_HOOK_H_
#define FINGERPRINTING_EVASIONS_HOOK_H_

#include <format>
#include <functional>
#include <map>
#include <random>
#include <string>
#include "base/component_export.h"
#include "fingerprinting/core/device_descriptor/fingerprint_impl.h"

namespace fingerprinting::core::evasions {

namespace {

namespace base {

class Value;
}
}  // namespace

enum HookTargetType {
  PAGE,
  WORKER,
  SHARED_WORKER,
};

class COMPONENT_EXPORT(FINGERPRINTING_CORE_EVASIONS) Hook {
  /*
   * Each hook represents a patch for a specific JS feature API
   * (e.g. WebGL, Canvas, etc.) A hook instance binds with a
   * specific target (e.g. Worker, Window, etc.)
   */

 public:
  Hook() = default;
  Hook(const Hook& other);
  Hook& operator=(const Hook&) = delete;

  // Hook(const Hook& other);
  // Hook& operator=(const Hook&) = delete;
  //   explicit Hook()
  //     : signature("f" + std::to_string(rand() % 1000000000000 +
  //     100000000000)) {
  // }

  virtual ~Hook() = default;

  // Returns the definition, but not the invocation
  // std::string get_definition();

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

  friend class HookFactory;

 private:
  std::string impl;
};

}  // namespace fingerprinting::core::evasions

#endif  // FINGERPRINTING_EVASIONS_HOOK_H_
