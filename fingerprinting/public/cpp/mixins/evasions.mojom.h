#ifndef FINGERPRINTING_EVASIONS_MIXIN_MOJOM_H
#define FINGERPRINTING_EVASIONS_MIXIN_MOJOM_H

#include "fingerprinting/evasions/packer.h"
#include "fingerprinting/export.h"
#include "fingerprinting/manager/manager.h"

namespace fingerprinting {

class GetEvasionsCallback;

class FINGERPRINTING_EXPORT EvasionsMixinMojom
    : virtual public FingerprintManagerCore {
 public:
  void GetEvasions(const evasions::HookTargetType target, GetEvasionsCallback callback) override;

 private:
  // TODO: Evasions codenames to disable, passed by --disable-evasions launch flag
  std::set<std::string> _disable_evasions;
};

}  // namespace fingerprinting

#endif  // FINGERPRINTING_EVASIONS_MIXIN_MOJOM_H
