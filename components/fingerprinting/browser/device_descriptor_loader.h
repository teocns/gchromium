#ifndef COMPONENTS_FINGERPRINTING_BROWSER_DEVICE_DESCRIPTOR_LOADER_H
#define COMPONENTS_FINGERPRINTING_BROWSER_DEVICE_DESCRIPTOR_LOADER_H

#include <any>
#include <format>
#include <mutex>
#include <string>
#include <unordered_map>
#include "base/component_export.h"
#include "base/files/file_util.h"
#include "base/json/json_reader.h"
#include "base/logging.h"
#include "base/strings/string_util.h"
#include "base/values.h"
#include "components/fingerprinting/common/device_descriptor_impl.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

#include "base/command_line.h"
#include "base/debug/stack_trace.h"
#include "base/process/process_handle.h"

namespace fingerprinting {

class COMPONENT_EXPORT(FINGERPRINTING_COMMON) DeviceDescriptorLoader {
 public:
  void Init();

  bool Loaded() { return loaded_; }

  bool Load(const std::string& file_path);

  DeviceDescriptor* GetPointer() { return ptr_.get(); }

 private:
  bool loaded_ = false;  // When the fingerprint is actually loaded

  // Usually set to false when switch isn't present
  bool can_be_initialized_ = true;

  std::unique_ptr<DeviceDescriptor> ptr_;
};

}  // namespace fingerprinting

#endif  // FINGERPRINTING_MANAGER_H_
