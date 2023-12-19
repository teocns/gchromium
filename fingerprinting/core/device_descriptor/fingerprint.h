#ifndef FINGERPRINTING_FINGERPRINT_H_
#define FINGERPRINTING_FINGERPRINT_H_

#include <vector>

#include "base/component_export.h"
#include "base/values.h"

namespace fingerprinting {


class COMPONENT_EXPORT(FINGERPRINTING_CORE_DEVICE_DESCRIPTOR) IFingerprint {
 public:
  virtual bool Find(std::vector<std::string>&& keys, base::Value*& out) = 0;
  virtual bool Find(std::vector<std::string>&& keys) = 0;
};

}  // namespace fingerprinting

#endif  // FINGERPRINTING_FINGERPRINT_H_
