#include "components/fingerprinting/renderer/evasions/hook_factory.h"
#include "components/fingerprinting/renderer/evasions/hook.h"
#include "base/logging.h"
namespace fingerprinting::core::evasions {
// std::unique_ptr<Hook> HookFactory::Create(const std::string& codename) {
//   auto it = GetRegistry().find(codename);
//   if (it != GetRegistry().end()) {
//     return it->second();
//   }
//   return nullptr;
// }

// std::unique_ptr<Hook> HookFactory::Create(const std::string& codename) {
//   auto registry = GetRegistry();
//   if (auto found = registry.Get(codename)) {
//     return found->value();
//   }
//   return nullptr;
// }

void HookFactory::Register(const std::string& key,
                           int priority,
                           HookConstructor constructor) {
  GetRegistry().Register(key, priority, constructor);
}

HookRegistry& HookFactory::GetRegistry() {
  // static std::map<std::string, HookConstructor> registry;
  static HookRegistry registry;
  return registry;
}

// Get Sorted Iterator

}  // namespace fingerprinting::core::evasions
