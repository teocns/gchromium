#ifndef COMPONENTS_FINGERPRINTING_COMMON_DEVICE_DESCRIPTOR_H
#define COMPONENTS_FINGERPRINTING_COMMON_DEVICE_DESCRIPTOR_H

#include <vector>

#include "base/component_export.h"
#include "base/values.h"

namespace fingerprinting {


class COMPONENT_EXPORT(FINGERPRINTING_COMMON_DEVICE_DESCRIPTOR) IDeviceDescriptor {
 public:
  virtual bool Find(std::vector<std::string>&& keys, base::Value*& out) = 0;
  virtual bool Find(std::vector<std::string>&& keys) = 0;
};

}  // namespace fingerprinting

#endif  // COMPONENTS_FINGERPRINTING_COMMON_DEVICE_DESCRIPTOR_H
