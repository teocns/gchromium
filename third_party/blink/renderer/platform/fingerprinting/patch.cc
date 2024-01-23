

#include "third_party/blink/renderer/platform/fingerprinting/patch.h"

namespace blink {

fingerprinting::Fingerprint* g_instance = nullptr;

// static
void FingerprintingResourceController::Set(fingerprinting::Fingerprint* instance) {
  g_instance = instance;
}

// static
fingerprinting::Fingerprint* FingerprintingResourceController::Get() {
  if (g_instance) {
    return g_instance;
  }
  return nullptr;
}

}  // namespace blink
