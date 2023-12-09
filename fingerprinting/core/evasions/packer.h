#include <set>
#include <string>
#include "fingerprinting/evasions/hook.h"
#include "fingerprinting/export.h"
namespace fingerprinting {
namespace evasions {

struct FINGERPRINTING_CORE_EXPORT Package {
  /*
   * Represents a packed collection of evasions ready to run as-a-script
   */

  HookTargetType target;
  explicit Package(HookTargetType target);
  void Register(std::unique_ptr<Hook> hook);

  static std::unique_ptr<Package> Pack(HookTargetType target,
                                       std::set<std::string>& filters);
};

}  // namespace evasions
}  // namespace fingerprinting
