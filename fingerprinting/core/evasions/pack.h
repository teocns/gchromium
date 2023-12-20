// fingerprint_manager.h
#ifndef FINGERPRINTING_EVASIONS_PACKER_H
#define FINGERPRINTING_EVASIONS_PACKER_H
#include <memory>
#include <set>
#include <string>
#include "fingerprinting/core/device_descriptor/fingerprint_impl.h"
#include "fingerprinting/core/evasions/hook.h"
namespace fingerprinting::core::evasions {

struct COMPONENT_EXPORT(FINGERPRINTING_CORE_EVASIONS) Package {
  /*
   * Represents a packed collection of evasions ready to run as-a-script
   */

 public:
  Package() = default;

  Package(const Package& other);
  Package& operator=(const Package&) = delete;

  explicit Package(HookTargetType target) : target(target) {}
  HookTargetType target;

  void Register(std::unique_ptr<Hook> hook) {
    this->hooks.push_back(std::move(hook));
  }

  static std::unique_ptr<Package> Pack(HookTargetType,
                      Fingerprint*,
                      std::set<std::string> = std::set<std::string>());

  std::vector<std::unique_ptr<Hook>> hooks;

  std::string get_iife();
};

}  // namespace fingerprinting::core::evasions

#endif  // FINGERPRINTING_EVASIONS_PACKER_H
