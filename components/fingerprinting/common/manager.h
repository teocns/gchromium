#ifndef COMPONENTS_FINGERPRINTING_COMMON_PROVIDER
#define COMPONENTS_FINGERPRINTING_COMMON_PROVIDER

#include <any>
#include <format>
#include <mutex>
#include <string>
#include <unordered_map>
#include "base/component_export.h"
#include "base/files/file_util.h"
#include "base/logging.h"
#include "base/no_destructor.h"
#include "base/values.h"
#include "components/fingerprinting/common/cache.h"
#include "components/fingerprinting/common/fingerprint_impl.h"
#include "third_party/abseil-cpp/absl/types/optional.h"
namespace fingerprinting {

class COMPONENT_EXPORT(FINGERPRINT_MANAGER) FingerprintManager {
  // Responsible for fetching the fingerprint from the browser process
  // and serving it to the renderer process
 public:
  bool isInitialized() { return this->fingerprint_ != nullptr; }

  static FingerprintManager* GetInstance() {
    static base::NoDestructor<FingerprintManager> instance;
    return instance.get();
  }

  Fingerprint* fingerprint() { return fingerprint_.get(); }

  bool CanBeInitialized() { return !initialized_ && can_be_initialized_; }

  void SetFingerprint(std::unique_ptr<Fingerprint> fp) {
    this->fingerprint_ = std::move(fp);
    this->initialized_ = true;
  }

  Fingerprint* GetFingerprint() { return this->fingerprint_.get(); }

  FingerprintManager();
  ~FingerprintManager();

 protected:
  Cache cache;

 private:
  std::unique_ptr<Fingerprint> fingerprint_ = nullptr;
  base::Lock lock_;

  bool initialized_ = false;        // When the fingerprint is initialized
  bool can_be_initialized_ = true;  // When the fingerprint can be initialized
};

}  // namespace fingerprinting

#endif  // FINGERPRINTING_MANAGER_H_
