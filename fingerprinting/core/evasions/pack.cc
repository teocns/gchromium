#include "fingerprinting/core/evasions/pack.h"
#include <vector>
#include "fingerprinting/core/evasions/hook.h"
#include "fingerprinting/core/evasions/hooks/webgl.h"

namespace fingerprinting::evasions {


void Package::Register(std::unique_ptr<Hook> hook) {
  this->hooks.push_back(std::move(hook));
}


std::unique_ptr<Package> Package::Pack(HookTargetType target,
                                       std::set<std::string>& filters) {
  // Returns a compiled, ready-to-inject JS function string
  // Filters are the evasions to disable
  auto pack = std::make_unique<Package>(target);

  for (auto& hook_descriptor : HookFactory::GetRegistry()) {
    const std::string& name = hook_descriptor.first;

    if (filters.find(name) != filters.end()) {
      // Skip disabled evasions
      continue;
    }

    std::unique_ptr<Hook> hook = HookFactory::Create(name);

    if (hook == nullptr) {
      continue;
    }

    pack->Register(std::move(hook));
  }

  return pack;
}

}  // namespace fingerprinting
