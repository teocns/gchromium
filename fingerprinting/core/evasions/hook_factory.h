
#ifndef FINGERPRINTING_CORE_EVASIONS_FACTORY_H
#define FINGERPRINTING_CORE_EVASIONS_FACTORY_H


#include <memory>
#include "base/component_export.h"
#include "fingerprinting/core/evasions/hook.h"


namespace fingerprinting::core::evasions {

typedef std::function<std::unique_ptr<Hook>()> HookConstructor;

/*
 * HookFactory is a factory class that creates Hook instances
 * based on a given key (e.g. "webgl", "canvas", etc.)
 * The key is used to lookup the registry of Hook constructors
 * (i.e. HookFactory::GetRegistry())
 * Yeah, I mixed factory & model in the same file, so what?
 */
class COMPONENT_EXPORT(FINGERPRINTING_CORE_EVASIONS) HookFactory {
 public:
  static void Register(const std::string& key, HookConstructor constructor);

  static std::unique_ptr<Hook> Create(const std::string& key);

  static std::map<std::string, HookConstructor>& GetRegistry();
};

// Macro to define a self-registering hook
#define REGISTER_HOOK(name, type)                                             \
  static bool _hook_##name##_registered =                                     \
      (HookFactory::Register(#name, [] { return std::make_unique<type>(); }), \
       true);


}  // namespace fingerprinting::core::evasions

#endif  // FINGERPRINTING_CORE_EVASIONS_FACTORY_H
