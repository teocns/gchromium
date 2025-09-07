#include <map>
#include <string>
#include "base/logging.h"
#include "base/no_destructor.h"
#include "base/strings/string_util.h"
#include "base/values.h"

#include "mojo/public/cpp/bindings/pending_receiver.h"
#include "mojo/public/cpp/bindings/receiver.h"  // Add this line

#include "base/process/process_handle.h"
#include "fingerprinting/core/device_descriptor/fingerprint_impl.h"
#include "fingerprinting/public/cpp/manager.h"

namespace fingerprinting {

FingerprintManager::FingerprintManager() = default;
FingerprintManager::~FingerprintManager() = default;

// static
FingerprintManager* FingerprintManager::GetInstance(bool try_init) {
  static base::NoDestructor<FingerprintManager> instance;

  // Keep track of equal-outputs to avoid flooding the logs
  FingerprintManager* ptr = instance.get();

#ifndef NDEBUG
  static std::map<std::string, bool> logs_history;
  std::string this_log = get_debug_info(ptr);

  if (!logs_history.contains(this_log)) {
    logs_history.emplace(this_log, true);
    DLOG(INFO) << this_log;
  }
#endif

  // Attempt initialization
  if (try_init && ptr->CanBeInitialized()) {
    ptr->Init();
  }
  return ptr;
}

void FingerprintManager::Enabled(EnabledCallback callback) {
  std::move(callback).Run(Loaded());
}

void FingerprintManager::Bind(
    mojo::PendingReceiver<fingerprinting::mojom::FingerprintManager> receiver) {
  receivers_.Add(this, std::move(receiver));
}

FingerprintManager* manager(bool try_init) {
  return FingerprintManager::GetInstance(try_init);
}

absl::optional<std::string> FingerprintManager::GetWebGLUnmaskedVendor() {
  base::Value* out = nullptr;
  if (!Loaded()) return absl::nullopt;
  Fingerprint* fp = fingerprint();
  if (!fp) return absl::nullopt;
  if (fp->Find({"webgl", "UNMASKED_VENDOR_WEBGL"}, out) && out && out->is_string())
    return out->GetString();
  if (fp->Find({"webgl", "vendor"}, out) && out && out->is_string())
    return out->GetString();
  if (fp->Find({"gpu", "vendor"}, out) && out && out->is_string())
    return out->GetString();
  return absl::nullopt;
}

absl::optional<std::string> FingerprintManager::GetWebGLUnmaskedRenderer() {
  base::Value* out = nullptr;
  if (!Loaded()) return absl::nullopt;
  Fingerprint* fp = fingerprint();
  if (!fp) return absl::nullopt;
  if (fp->Find({"webgl", "UNMASKED_RENDERER_WEBGL"}, out) && out && out->is_string())
    return out->GetString();
  if (fp->Find({"webgl", "renderer"}, out) && out && out->is_string())
    return out->GetString();
  if (fp->Find({"gpu", "renderer"}, out) && out && out->is_string())
    return out->GetString();
  return absl::nullopt;
}

}  // namespace fingerprinting
