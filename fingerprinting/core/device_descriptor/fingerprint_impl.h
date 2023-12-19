#ifndef FINGERPRINTING_FINGERPRINT_IMPL_H_
#define FINGERPRINTING_FINGERPRINT_IMPL_H_

#include <vector>

#include "base/component_export.h"
#include "fingerprinting/core/device_descriptor/fingerprint.h"
#include "fingerprinting/core/device_descriptor/mixins/user-agent.h"

namespace fingerprinting {

class COMPONENT_EXPORT(FINGERPRINTING_CORE_DEVICE_DESCRIPTOR) Fingerprint final
    : public UAMixin,
      protected virtual IFingerprint {
 public:
  Fingerprint& operator=(Fingerprint&& other);
  Fingerprint(Fingerprint&& other);

  // Destructor
  ~Fingerprint();
  explicit Fingerprint(base::Value& value);

  explicit Fingerprint(base::Value&& value);

  bool Find(std::vector<std::string>&& keys, base::Value*& out) override;
  bool Find(std::vector<std::string>&& keys) override;
  static bool FromString(const std::string& fingerprint_str,
                         std::unique_ptr<Fingerprint>& out);

 private:
  base::Value value_;
  std::unordered_map<std::size_t, base::Value*> cache_;

  std::size_t hash_keys(std::vector<std::string>& keys);

  bool find_in_cache(const std::size_t& hash, base::Value*& out);

  bool find_(std::vector<std::string>& keys, base::Value*&);
};

}  // namespace fingerprinting

#endif  // FINGERPRINTING_FINGERPRINT_IMPL_H_
