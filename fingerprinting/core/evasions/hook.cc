#include "fingerprinting/core/evasions/hook.h"
#include "v8/include/v8-context.h"
namespace fingerprinting::evasions {

std::string Hook::get_definition(HookTargetType target) {
  /*
   * Geneerates the JavaScript definition of the "hook" function
   * A random name will be generated for
   */
  return std::format("{} = function(){{ {} }};", this->signature,
                     this->get_impl(target));
}

std::string Hook::get_iife(HookTargetType target) {
  return std::format("({})()", this->get_definition(target));
}

std::unique_ptr<Hook> HookFactory::Create(const std::string& key) {
  auto it = GetRegistry().find(key);
  if (it != GetRegistry().end()) {
    return it->second();
  }
  return nullptr;
}

void HookFactory::Register(const std::string& key,
                           HookConstructor constructor) {
  GetRegistry()[key] = constructor;
}

std::map<std::string, HookConstructor>& HookFactory::GetRegistry() {
  static std::map<std::string, HookConstructor> registry;
  return registry;
}

}  // namespace fingerprinting::evasions
