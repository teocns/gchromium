#include <set>
#include <string>
#include "fingerprinting/evasions/export.h"
#include "fingerprinting/evasions/hook.h"
namespace fingerprinting {
namespace evasions {

class FINGERPRINTING_EVASIONS_EXPORT Package {
  /*
   * Represents a packed collection of evasions ready to run as-a-script
   */

 public:
  HookTarget target;
  explicit Package(HookTargetType target);
  void Register(std::unique_ptr<Hook> hook);
  void Inject(v8::Local<v8::Context> context);

  static std::unique_ptr<Package> Pack(HookTargetType target,
                                       std::set<std::string>& filters);
};

}  // namespace evasions
}  // namespace fingerprinting
