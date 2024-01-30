#include "components/fingerprinting/renderer/evasions/pack.h"
#include <vector>
#include "base/logging.h"
#include "components/fingerprinting/renderer/evasions/hook.h"
#include "components/fingerprinting/renderer/evasions/hook_factory.h"

// Include all hooks here
#include "components/fingerprinting/renderer/evasions/hooks/navigator-battery-manager.h"
#include "components/fingerprinting/renderer/evasions/hooks/navigator-webdriver.h"
#include "components/fingerprinting/renderer/evasions/hooks/navigator.h"
#include "components/fingerprinting/renderer/evasions/hooks/utils.h"
#include "components/fingerprinting/renderer/evasions/hooks/webgl.h"

namespace fingerprinting::core::evasions {

std::string EvasionsPackage::get_iife() {
  std::string iife = "(function(){";
  for (auto& hook : this->hooks) {
    iife += hook->get_iife();
  }
  iife += "})();";
  return iife;
}

std::unique_ptr<EvasionsPackage> EvasionsPackage::Pack(
    HookTargetType target,
    std::set<std::string> filters) {
  // Returns a compiled, ready-to-inject JS function string
  // Filters are the evasions to disable

  auto pack = std::make_unique<EvasionsPackage>(target);
  for (auto& entry : HookFactory::GetRegistry()) {
    const std::string& name = entry.key;

    if (filters.find(name) != filters.end()) {
      // Skip disabled evasions
      continue;
    }

    std::unique_ptr<Hook> hook = entry.value();
    if (hook == nullptr) {
      LOG(ERROR) << "Failed to create hook " << name;
      continue;
    }

    // Skip hooks that are not allowed for the target
    // Note this is a bitwise operation
    if ((hook->AllowedTargetTypes() & target) == HookTargetType::NONE) {
      LOG(INFO) << "Skipping hook " << name;
      continue;
    }
    pack->Register(std::move(hook));
  }

  return pack;
}

}  // namespace fingerprinting::core::evasions
