#ifndef FINGERPRINTING_FINGERPRINT_H_
#define FINGERPRINTING_FINGERPRINT_H_

#include <vector>

#include "base/component_export.h"
#include "base/values.h"

namespace fingerprinting {

using Val = base::Value;

class COMPONENT_EXPORT(FINGERPRINTING_CORE) IFingerprint {
 public:
  virtual bool Find(std::vector<std::string>&& keys, Val*& out) = 0;
  virtual bool Find(std::vector<std::string>&& keys) = 0;
};

}  // namespace fingerprinting

#endif  // FINGERPRINTING_FINGERPRINT_H_
