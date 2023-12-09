#include "fingerprinting/evasions/packer.h"
#include <vector>
#include "fingerprinting/evasions/hook.h"
#include "fingerprinting/evasions/hooks/webgl.cc"

namespace fingerprinting {
namespace evasions {

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

}  // namespace evasions

}  // namespace fingerprinting
