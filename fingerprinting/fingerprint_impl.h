#ifndef FINGERPRINTING_FINGERPRINT_IMPL_H_
#define FINGERPRINTING_FINGERPRINT_IMPL_H_

#include <vector>

#include "base/values.h"

#include "fingerprinting/export.h"

#include "fingerprinting/fingerprint.h"
#include "fingerprinting/mixins/user-agent.h"

namespace fingerprinting {

class FINGERPRINTING_EXPORT Fingerprint final : public UAMixin,
                                                protected virtual IFingerprint {
 public:
  Fingerprint& operator=(Fingerprint&& other);
  Fingerprint(Fingerprint&& other);

  // Destructor
  ~Fingerprint();
  Fingerprint(Val& value);

  Fingerprint(Val&& value);

  bool Find(std::vector<std::string>&& keys, Val*& out) override;
  bool Find(std::vector<std::string>&& keys) override;
  static bool FromString(const std::string& fingerprint_str,
                         std::unique_ptr<Fingerprint>& out);

 private:
  Val value_;
  std::unordered_map<std::size_t, Val*> cache_;

  std::size_t hash_keys(std::vector<std::string>& keys);

  bool find_in_cache(const std::size_t& hash, Val*& out);

  bool find_(std::vector<std::string>& keys, Val*&);
};

}  // namespace fingerprinting

#endif  // FINGERPRINTING_FINGERPRINT_IMPL_H_
