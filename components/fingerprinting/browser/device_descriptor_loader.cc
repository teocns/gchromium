#include "components/fingerprinting/browser/device_descriptor_loader.h"
#include <string>
#include "base/files/file_path.h"
#include "base/files/file_util.h"
#include "base/logging.h"
#include "components/fingerprinting/common/device_descriptor_impl.h"
namespace fingerprinting {

bool DeviceDescriptorLoader::Load(const std::string& fingerprint_file_path) {
  std::string dd_str;

  // Convert std::string to base::FilePath
  base::FilePath fingerprint_file_path_o =
      base::FilePath(fingerprint_file_path);

  if (!base::ReadFileToString(fingerprint_file_path_o, &dd_str)) {
    LOG(ERROR) << "Failed reading: " << fingerprint_file_path;
    return false;
  }

  if (!DeviceDescriptor::FromString(dd_str, ptr_)) {
    LOG(ERROR) << std::format("Failed serializing JSON {}",
                              fingerprint_file_path);
    return false;
  }

  loaded_ = true;
  // Print the fingerprint_file_path location
  LOG(INFO) << "Loaded --fingerprint " << fingerprint_file_path;
  return true;
}
}  // namespace fingerprinting
