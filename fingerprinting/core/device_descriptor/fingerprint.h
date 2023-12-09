#ifndef FINGERPRINTING_FINGERPRINT_H_
#define FINGERPRINTING_FINGERPRINT_H_

#include <vector>

#include "base/values.h"
#include "fingerprinting/core/export.h"

namespace fingerprinting {

using Val = base::Value;

class FINGERPRINTING_CORE_EXPORT IFingerprint {
 public:
  virtual bool Find(std::vector<std::string>&& keys, Val*& out) = 0;
  virtual bool Find(std::vector<std::string>&& keys) = 0;
};

}  // namespace fingerprinting

#endif  // FINGERPRINTING_FINGERPRINT_H_
