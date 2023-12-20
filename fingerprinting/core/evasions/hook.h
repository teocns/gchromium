#ifndef FINGERPRINTING_EVASIONS_HOOK_H_
#define FINGERPRINTING_EVASIONS_HOOK_H_

#include <format>
#include <functional>
#include <map>
#include <random>
#include <string>
#include "base/component_export.h"

namespace fingerprinting::core::evasions {





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
  Hook(const std::string& codename, const std::string& impl) : codename(codename), impl(impl) {}
  //   explicit Hook()
  //     : signature("f" + std::to_string(rand() % 1000000000000 + 100000000000)) {
  // }

  virtual ~Hook() = default;
  /*
   * An ephemeral function name that will represent the hook
   * No parameters are defined: in JavaScript, arguments can be referenced via
   * builtin `arguments` variable (e.g. arguments[0], arguments[1], etc.)
   */
  std::string signature;

  // Returns the definition, but not the invocation
  std::string get_definition();

  // Returns immediately-invoked function expression (IIFE)
  std::string get_iife();

  std::string codename;
  /*
   * The implementation [body inner part] of the the hook function definition
   * (aka routine) This is where the actual patch logic is defined by
   * implementors of `Hook` (subclasses)
   */
  virtual std::string get_impl() { return this->impl; }

  friend class HookFactory;

 private:
  std::string impl;
};





}  // namespace fingerprinting::core::evasions

#endif  // FINGERPRINTING_EVASIONS_HOOK_H_
