
#ifndef FINGERPRINTING_CORE_EVASIONS_FACTORY_H
#define FINGERPRINTING_CORE_EVASIONS_FACTORY_H

#include <algorithm>
#include <functional>
#include <memory>
#include "base/component_export.h"
#include "components/fingerprinting/renderer/evasions/hook.h"
#include "components/fingerprinting/renderer/evasions/hook_registry.h"

namespace fingerprinting::core::evasions {

/*
 * HookFactory is a factory class that creates Hook instances
 * based on a given key (e.g. "webgl", "canvas", etc.)
 * The key is used to lookup the registry of Hook constructors
 * (i.e. HookFactory::GetRegistry())
 */

class HookFactory {
 public:
  static void Register(const std::string& key,
                       int priority,
                       HookConstructor constructor);

  // static std::unique_ptr<Hook> Create(const std::string& key);

  static HookRegistry& GetRegistry();
};

// Macro to define a self-registering hook
// TODO remove pseudo priority value
#define REGISTER_HOOK(name, type, priority)                                    \
  static bool _hook_##name##_registered =                                      \
      (HookFactory::Register(#name, priority,                                  \
                             [] { return std::make_unique<type>(priority); }), \
       true);

}  // namespace fingerprinting::core::evasions

#endif  // FINGERPRINTING_CORE_EVASIONS_FACTORY_H
