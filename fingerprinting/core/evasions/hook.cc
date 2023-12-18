#include "fingerprinting/core/evasions/hook.h"
namespace fingerprinting::core::evasions {

std::string Hook::get_definition() {
  /*
   * Geneerates the JavaScript definition of the "hook" function
   * A random name will be generated for
   */
  return std::format("{} = function(){{ {} }};", this->codename,
                     this->get_impl());
}

std::string Hook::get_iife() {
#ifdef NDEBUG
  return std::format("(function(){{ {} }})()", get_impl(target));
#else
  return std::format("(function(){{ console.info('Running hook {}'); {} }})()", codename, get_impl());
#endif
}

std::unique_ptr<Hook> HookFactory::Create(const std::string& codename) {
  auto it = GetRegistry().find(codename);
  if (it != GetRegistry().end()) {
    return it->second();
  }
  return nullptr;
}


std::unique_ptr<Hook> HookFactory::Create(const std::string& codename, const std::string impl) {
  auto it = GetRegistry().find(codename);
  if (it != GetRegistry().end()) {
    auto hook = it->second();
    hook->impl = impl;
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

}  // namespace fingerprinting::core::evasions
