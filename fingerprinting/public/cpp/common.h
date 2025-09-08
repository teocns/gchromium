
#ifndef FINGERPRINTING_PUBLIC_COMMON_H
#define FINGERPRINTING_PUBLIC_COMMON_H

#include "fingerprinting/core/manager.h"
#include "fingerprinting/public/cpp/export.h"
#include "fingerprinting/public/mojom/manager.mojom.h"

namespace fingerprinting::internal {

class FINGERPRINTING_PUBLIC_EXPORT FingerprintManagerBase
    : public fingerprinting::core::FingerprintManagerCore,
      virtual public fingerprinting::mojom::FingerprintManager {};

}  // namespace fingerprinting::internal

#endif  // FINGERPRINTING_PUBLIC_COMMON_H
