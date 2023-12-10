// fingerprint_manager.h
#ifndef FINGERPRINTING_EVASIONS_PACKER_H
#define FINGERPRINTING_EVASIONS_PACKER_H
#include <set>
#include <string>
#include "fingerprinting/core/evasions/hook.h"
#include "fingerprinting/core/export.h"
namespace fingerprinting::evasions {

struct FINGERPRINTING_CORE_EXPORT Package {
  /*
   * Represents a packed collection of evasions ready to run as-a-script
   */



  explicit Package(HookTargetType target) : target(target) {}
  HookTargetType target;
  void Register(std::unique_ptr<Hook> hook);

  static std::unique_ptr<Package> Pack(HookTargetType target,
                                       std::set<std::string>& filters);


private:
  std::vector<std::unique_ptr<Hook>> hooks;
};

}  // namespace fingerprinting

#endif  // FINGERPRINTING_EVASIONS_PACKER_H
