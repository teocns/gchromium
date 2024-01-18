#include "components/fingerprinting/common/manager.h"
#include <format>
#include <string>
#include "base/command_line.h"
#include "base/files/file_util.h"
#include "base/logging.h"
#include "base/no_destructor.h"
#include "base/strings/string_util.h"
#include "base/synchronization/lock.h"
#include "base/values.h"
#include "components/fingerprinting/common/device_descriptor/fingerprint_impl.h"
#include "mojo/public/cpp/bindings/pending_receiver.h"
#include "mojo/public/cpp/bindings/receiver.h"  // Add this line
#include "mojo/public/cpp/bindings/receiver_set.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

namespace fingerprinting::core {

FingerprintManagerCore::FingerprintManagerCore() = default;
FingerprintManagerCore::~FingerprintManagerCore() = default;

void FingerprintManagerCore::Init() {
  base::AutoLock auto_lock(lock_);
  if (!CanBeInitialized()) {
    return;
  }

  std::string fingerprint_file_path;
  if (base::CommandLine::ForCurrentProcess()->HasSwitch("fingerprint")) {
    fingerprint_file_path =
        base::CommandLine::ForCurrentProcess()->GetSwitchValueASCII(
            "fingerprint");
  } else {
    LOG(WARNING)
        << "No --fingerprint switch passed. Using browser's real fingerprint.";
    SetCanBeInitialized(false);
    return;
  }

#ifndef NDEBUG
  DLOG(INFO) << std::format("Initializing fingerprint from: {}\n{}",
                            fingerprint_file_path, get_debug_info(this));
#endif
  initialized_ = true;
  LoadFingerprint(fingerprint_file_path);
}

void FingerprintManagerCore::LoadFingerprint(
    const std::string& fingerprint_file_path) {
  std::string fingerprint_str;

  // Convert std::string to base::FilePath
  base::FilePath fingerprint_file_path_o =
      base::FilePath(fingerprint_file_path);
  if (!base::ReadFileToString(fingerprint_file_path_o, &fingerprint_str)) {
    LOG(ERROR) << "Failed reading: " << fingerprint_file_path;
    return;
  }

  if (!Fingerprint::FromString(fingerprint_str, fingerprint_)) {
    LOG(ERROR) << std::format("Failed serializing JSON {}",
                              fingerprint_file_path);
    return;
  }

  loaded_ = true;
  // Print the fingerprint_file_path location
  LOG(INFO) << "Loaded --fingerprint " << fingerprint_file_path;
}

Fingerprint* FingerprintManagerCore::GetFingerprint() {
  return fingerprint_.get();
}

}  // namespace fingerprinting::core
