
#include <map>
#include <string>
#include "base/logging.h"
#include "base/no_destructor.h"
#include "base/strings/string_util.h"
#include "base/values.h"

#include "mojo/public/cpp/bindings/pending_receiver.h"
#include "mojo/public/cpp/bindings/receiver.h"  // Add this line

#include "base/synchronization/lock.h"
#include "base/process/process_handle.h"
#include "components/fingerprinting/browser/manager_impl.h"
#include "components/fingerprinting/common/fingerprint_impl.h"

namespace fingerprinting {

// FingerprintManagerImpl::FingerprintManagerImpl() = default;
// FingerprintManagerImpl::~FingerprintManagerImpl() = default;

// static
FingerprintManagerImpl* FingerprintManagerImpl::GetInstance(bool try_init) {
  static base::NoDestructor<FingerprintManagerImpl> instance;

  // Keep track of equal-outputs to avoid flooding the logs
  FingerprintManagerImpl* ptr = instance.get();

  // Attempt initialization
  if (try_init && ptr->CanBeInitialized()) {
    ptr->MaybeInitialize();
  }
  return ptr;
}

void FingerprintManagerImpl::Enabled(EnabledCallback callback) {
  std::move(callback).Run(isInitialized());
}
void FingerprintManagerImpl::GetFingerprintString(
    GetFingerprintStringCallback callback) {
  std::move(callback).Run(this->GetFingerprint()->str_value());
}

void FingerprintManagerImpl::AddReceiver(
    mojo::PendingReceiver<fingerprinting::mojom::FingerprintManager> receiver) {
  receivers_.Add(this, std::move(receiver));
}

void FingerprintManagerImpl::GetFingerprintValue(
    GetFingerprintValueCallback callback) {
  std::move(callback).Run(this->GetFingerprint()->value().Clone());
}

void MaybeInitialize() {
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
    this->can_be_initialized_ = false;
    return;
  }

#ifndef NDEBUG
  DLOG(INFO) << std::format("Initializing fingerprint from {}",
                            fingerprint_file_path);
#endif
  initialized_ = true;
  LoadFingerprint(fingerprint_file_path);
}
}  // namespace fingerprinting
