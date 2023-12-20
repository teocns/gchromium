#include "fingerprinting/core/evasions/pack.h"
#include <vector>
#include "fingerprinting/core/evasions/hook.h"
#include "fingerprinting/core/evasions/hooks/webgl.h"

namespace fingerprinting::core::evasions {


std::string Package::get_iife() {
  std::string iife = "(function(){";
  for (auto& hook : this->hooks) {
    iife += hook.get_iife();
  }
  iife += "})();";
  return iife;
}

Package Package::Pack(HookTargetType target, std::shared_ptr<Fingerprint> fingerprint, std::set<std::string> filters) {
  // Returns a compiled, ready-to-inject JS function string
  // Filters are the evasions to disable
  Package pack(target, fingerprint);

  for (auto& hook_descriptor : HookFactory::GetRegistry()) {
    const std::string& name = hook_descriptor.first;

    if (filters.find(name) != filters.end()) {
      // Skip disabled evasions
      continue;
    }

    std::unique_ptr<Hook> hook = HookFactory::Create(name);
    if (hook != nullptr) {
      pack.Register(std::move(hook));
    }
  }

  return pack;
}

}  // namespace fingerprinting::core::evasions
