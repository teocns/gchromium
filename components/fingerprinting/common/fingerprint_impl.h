#ifndef FINGERPRINTING_FINGERPRINT_IMPL_H_
#define FINGERPRINTING_FINGERPRINT_IMPL_H_

#include <vector>

#include "base/component_export.h"
#include "components/fingerprinting/common/fingerprint.h"
#include "components/fingerprinting/common/mixins/user-agent.h"
#include "components/fingerprinting/common/mixins/webgl.h"

namespace fingerprinting {

class COMPONENT_EXPORT(COMPONENTS_FINGERPRINTING_COMMON_FINGERPRINT)
    Fingerprint final : public UAMixin,
                        public WebGLMixin,
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

  base::Value& value() { return this->value_; }
  std::string& str_value() { return this->str_value_; }

  // Statics
  static absl::optional<Fingerprint>& From(const std::string& fingerprint_str);

 private:
  base::Value value_;
  std::string str_value_;
  std::unordered_map<std::size_t, base::Value*> cache_;

  std::size_t hash_keys(std::vector<std::string>& keys);

  bool find_in_cache(const std::size_t& hash, base::Value*& out);

  bool find_(std::vector<std::string>& keys, base::Value*&);
};

}  // namespace fingerprinting

#endif  // FINGERPRINTING_FINGERPRINT_IMPL_H_
