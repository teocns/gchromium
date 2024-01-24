#include "components/fingerprinting/renderer/evasions/hook.h"
#include "components/fingerprinting/renderer/evasions/hook_factory.h"
namespace fingerprinting::core::evasions {
std::unique_ptr<Hook> HookFactory::Create(const std::string& codename) {
  auto it = GetRegistry().find(codename);
  if (it != GetRegistry().end()) {
    return it->second();
  }
  return nullptr;
}

// std::unique_ptr<Hook> HookFactory::Create(const std::string& codename,
//                                           const std::string impl) {
//   auto it = GetRegistry().find(codename);
//   if (it != GetRegistry().end()) {
//     auto hook = it->second();
//     hook->impl = impl;
//   }
//   return nullptr;
// }

void HookFactory::Register(const std::string& key,
                           HookConstructor constructor) {
  GetRegistry()[key] = constructor;
}

std::map<std::string, HookConstructor>& HookFactory::GetRegistry() {
  static std::map<std::string, HookConstructor> registry;
  return registry;
}

}  // namespace fingerprinting::core::evasions
