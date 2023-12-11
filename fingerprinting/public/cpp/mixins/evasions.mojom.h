#ifndef FINGERPRINTING_EVASIONS_MIXIN_MOJOM_H
#define FINGERPRINTING_EVASIONS_MIXIN_MOJOM_H

#include "fingerprinting/core/evasions/pack.h"
#include "fingerprinting/public/cpp/common.h"
#include "fingerprinting/public/cpp/export.h"

namespace fingerprinting::internal {


class FINGERPRINTING_PUBLIC_EXPORT EvasionsMixinMojom
    : virtual public internal::FingerprintManagerBase {
 public:
  void GetEvasions(mojom::HookTargetType target, mojom::FingerprintManager::GetEvasionsCallback callback) override;

 private:
  // TODO: Evasions codenames to disable, passed by --disable-evasions launch flag
  std::set<std::string> _disable_evasions;
};

}  // namespace fingerprinting

#endif  // FINGERPRINTING_EVASIONS_MIXIN_MOJOM_H
