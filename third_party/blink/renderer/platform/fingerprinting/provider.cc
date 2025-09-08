#include "third_party/blink/renderer/platform/fingerprinting/provider.h"

namespace blink {

fingerprinting::Fingerprint* g_instance = nullptr;

// static
void FingerprintProvider::Set(fingerprinting::Fingerprint* instance) {
  g_instance = instance;
}

// static
fingerprinting::Fingerprint* FingerprintProvider::Get() {
  if (g_instance) {
    return g_instance;
  }
  return nullptr;
}

}  // namespace blink
