#include <string>
#include "fingerprinting/manager.h"
#include "fingerprinting/manager.mojom.h"
#include "base/files/file_util.h"
#include "base/json/json_reader.h"
#include "base/logging.h"
#include "base/no_destructor.h"
#include "base/strings/string_util.h"
#include "base/synchronization/lock.h"
#include "base/values.h"
#include "third_party/abseil-cpp/absl/types/optional.h"
#include "mojo/public/cpp/bindings/pending_receiver.h"
#include "mojo/public/cpp/bindings/receiver.h"  // Add this line
#include "mojo/public/cpp/bindings/receiver_set.h"
#include "base/command_line.h"

namespace fingerprinting {



FingerprintManager::FingerprintManager() = default;
FingerprintManager::~FingerprintManager() = default;

// static
FingerprintManager* FingerprintManager::GetInstance() {
  static base::NoDestructor<FingerprintManager> instance;
  return instance.get();
}

  void FingerprintManager::Init() {
    base::AutoLock auto_lock(lock_);

    LOG(INFO) << "FingerprintManager::Init";

    std::string fingerprint_file_path;
    if (base::CommandLine::ForCurrentProcess()->HasSwitch("fingerprint")) {
				LOG(INFO) << "HasSwitch";
        fingerprint_file_path = base::CommandLine::ForCurrentProcess()->GetSwitchValueASCII("fingerprint");
    } else {
      LOG(INFO) << "NoSwitch";
      SetCanBeInitialized(false);
      return;
    }

    
    if (!CanBeInitialized()) {
      LOG(INFO) << "FingerprintManager::Init - Cannot be initialized";
      return;
    }
    initialized_ = true;

    LOG(INFO) << "FingerprintManager::Init -Can be initialized";
    LoadFingerprint(fingerprint_file_path);
  }

void FingerprintManager::Enabled(EnabledCallback callback) {
  std::move(callback).Run(loaded_);
}

void FingerprintManager::GetFingerprintStr(GetFingerprintStrCallback callback) {
  base::AutoLock auto_lock(lock_);
  LOG(INFO) << "FingerprintManager::GetFingerprintStr";
  std::move(callback).Run(fingerprint_str_);
}


void FingerprintManager::SetFingerprintStr(const std::string& fingerprint, SetFingerprintStrCallback callback) {
  base::AutoLock auto_lock(lock_);

  absl::optional<Fingerprint> fp = FingerprintManager::FromStringToObj(fingerprint);

  if (!fp.has_value()) {
    std::move(callback).Run(false);
  }
  fingerprint_str_ = std::move(fingerprint);
  fingerprint_ = std::make_unique<Fingerprint>(std::move(fp.value()));

  std::move(callback).Run(true);
}


void FingerprintManager::LoadFingerprint(const std::string& fingerprint_file_path) {
  std::string fingerprint_str;

  // Convert std::string to base::FilePath
  base::FilePath fingerprint_file_path_o = base::FilePath(fingerprint_file_path);
  if (!base::ReadFileToString(fingerprint_file_path_o, &fingerprint_str)) {
    LOG(ERROR) << "Failed to read fingerprint file: " << fingerprint_file_path;
    return;
  }

  absl::optional<Fingerprint> fp = FingerprintManager::FromStringToObj(fingerprint_str);

  if (fp.has_value()) {
    fingerprint_ = std::make_unique<Fingerprint>(std::move(fp.value()));
    fingerprint_str_ = std::move(fingerprint_str);
    loaded_ = true;
    // Print the fingerprint_file_path location
    LOG(INFO) << "Loaded fingerprint from " << fingerprint_file_path;
  }
}

absl::optional<Fingerprint> FingerprintManager::FromStringToObj(const std::string& fingerprint_str) {
  absl::optional<base::Value> fingerprint_value = 
      base::JSONReader::Read(fingerprint_str);

  if (!fingerprint_value.has_value()) {
    LOG(ERROR) << "Failed to parse fingerprint JSON: " << fingerprint_str;
    return absl::nullopt;
  }
  
  base::Value& fingerprint_dict = fingerprint_value.value();

  if (!fingerprint_dict.is_dict()) {
    LOG(ERROR) << "Fingerprint JSON is not a dictionary: " << fingerprint_str;
    return absl::nullopt;
  }

  return Fingerprint(std::move(fingerprint_dict));
}


void FingerprintManager::Bind(mojo::PendingReceiver<fingerprinting::mojom::FingerprintManager> receiver) {
  receivers_.Add(this, std::move(receiver));
}

}  // namespace fingerprinting