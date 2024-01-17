#ifndef COMPONENTS_FINGERPRINTING_COMMON_DEVICE_DESCRIPTOR_IMPL_H
#define COMPONENTS_FINGERPRINTING_COMMON_DEVICE_DESCRIPTOR_IMPL_H

#include <vector>

#include "base/component_export.h"
#include "components/fingerprinting/common/device_descriptor.h"
#include "components/fingerprinting/common/device_descriptor_mixins/user-agent.h"
#include "components/fingerprinting/common/device_descriptor_mixins/webgl.h"

namespace fingerprinting {

class COMPONENT_EXPORT(FINGERPRINTING_COMMON_DEVICE_DESCRIPTOR)
    DeviceDescriptor final : public UAMixin,
                             public WebGLMixin,
                             protected virtual IDeviceDescriptor {
 public:
  DeviceDescriptor& operator=(DeviceDescriptor&& other);
  DeviceDescriptor(DeviceDescriptor&& other);

  // Destructor
  ~DeviceDescriptor();
  explicit DeviceDescriptor(base::Value& value);

  explicit DeviceDescriptor(base::Value&& value);

  bool Find(std::vector<std::string>&& keys, base::Value*& out) override;
  bool Find(std::vector<std::string>&& keys) override;
  static bool FromString(const std::string& fingerprint_str,
                         std::unique_ptr<DeviceDescriptor>& out);

  base::Value& value() { return this->value_; }
  std::string& str_value() { return this->str_value_; }

 private:
  base::Value value_;
  std::string str_value_;
  std::unordered_map<std::size_t, base::Value*> cache_;

  std::size_t hash_keys(std::vector<std::string>& keys);

  bool find_in_cache(const std::size_t& hash, base::Value*& out);

  bool find_(std::vector<std::string>& keys, base::Value*&);
};

}  // namespace fingerprinting

#endif  // COMPONENTS_FINGERPRINTING_COMMON_DEVICE_DESCRIPTOR_IMPL_H
