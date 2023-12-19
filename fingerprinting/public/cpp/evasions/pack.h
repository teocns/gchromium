
// fingerprint_manager.h
#ifndef FINGERPRINTING_PUBLIC_EVASIONS_PACK_H
#define FINGERPRINTING_PUBLIC_EVASIONS_PACK_H
#include <memory>
#include <set>
#include <string>
#include <vector>
#include "fingerprinting/core/device_descriptor/fingerprint_impl.h"
#include "fingerprinting/core/evasions/hook.h"
#include "fingerprinting/public/cpp/evasions/hook.h"

#include "v8/include/v8-context.h"

namespace fingerprinting::evasions {

struct COMPONENT_EXPORT(FINGERPRINTING_PUBLIC_EVASIONS) Package
    : virtual ::fingerprinting::core::evasions::Package {
  /*
   * Represents a packed collection of evasions ready to run as-a-script
   * with context of V8 
   */

 public:
  void Inject(v8::Local<v8::Context> context);

  ::fingerprinting::Fingerprint fingerprint;


  std::vector<Hook> hooks;

};

}  // namespace fingerprinting::evasions

#endif  // FINGERPRINTING_PUBLIC_EVASIONS_PACK_H
