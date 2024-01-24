// fingerprint_manager.h
#ifndef COMPONENTS_FINGERPRINTING_EVASIONS_PACKER_H
#define COMPONENTS_FINGERPRINTING_EVASIONS_PACKER_H
#include <memory>
#include <set>
#include <string>
#include "fingerprinting/core/device_descriptor/fingerprint_impl.h"
#include "fingerprinting/core/evasions/hook.h"
namespace fingerprinting::core::evasions {

struct COMPONENT_EXPORT(FINGERPRINTING_CORE_EVASIONS) EvasionsPackage {
  /*
   * Represents a packed collection of evasions ready to run as-a-script
   */

 public:
  EvasionsPackage() = default;

  EvasionsPackage(const EvasionsPackage& other) = default;

  // Other constructors default
  EvasionsPackage(EvasionsPackage&& other) = default;
  EvasionsPackage& operator=(const EvasionsPackage& other) = default;
  EvasionsPackage& operator=(EvasionsPackage&& other) = default;

  explicit EvasionsPackage(HookTargetType target) : target_(target) {}

  HookTargetType target_;

  void Register(std::unique_ptr<Hook> hook) {
    this->hooks.push_back(std::move(hook));
  }

  static std::unique_ptr<EvasionsPackage> Pack(
      HookTargetType,
      std::set<std::string> = std::set<std::string>());

  std::vector<std::unique_ptr<Hook>> hooks;

  std::string get_iife();
};

}  // namespace fingerprinting::core::evasions

#endif  // COMPONENTS_FINGERPRINTING_EVASIONS_PACKER_H
