#ifndef FINGERPRINTING_FINGERPRINT_H_
#define FINGERPRINTING_FINGERPRINT_H_

#include <map>
#include <vector>

#include "base/values.h"


#include "third_party/abseil-cpp/absl/types/optional.h"

#include "fingerprinting/export.h"

namespace fingerprinting {

using Val = base::Value;

class FINGERPRINTING_EXPORT IFingerprint {
 public:

  virtual bool Find(std::vector<std::string>&& keys, Val*& out) = 0;
  virtual bool Find(std::vector<std::string>&& keys) = 0;

};

}  // namespace fingerprinting

#endif  // FINGERPRINTING_FINGERPRINT_H_
