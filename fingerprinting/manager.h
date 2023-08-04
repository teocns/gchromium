// fingerprint_manager.h
#ifndef FINGERPRINTING_MANAGER_H_
#define FINGERPRINTING_MANAGER_H_

#include <string>

#include "base/files/file_util.h"
#include "base/json/json_reader.h"
#include "base/logging.h"
#include "base/no_destructor.h"
#include "base/strings/string_util.h"
#include "base/synchronization/lock.h"
#include "base/values.h"
#include "fingerprinting/export.h"
#include "fingerprinting/manager.mojom.h"
#include "fingerprinting/fingerprint.h"
#include "third_party/abseil-cpp/absl/types/optional.h"



#include "mojo/public/cpp/bindings/receiver.h"  // Add this line
#include "mojo/public/cpp/bindings/receiver_set.h"

namespace fingerprinting {

class FINGERPRINTING_EXPORT FingerprintManager : public fingerprinting::mojom::FingerprintManager {
 public:
  static FingerprintManager* GetInstance();

  void Init();

  void SetFingerprintStr(const std::string& fingerprint, SetFingerprintStrCallback callback) override;

  void GetFingerprintStr(GetFingerprintStrCallback callback) override;

  void Enabled(EnabledCallback callback) override;


  void Bind(mojo::PendingReceiver<fingerprinting::mojom::FingerprintManager> receiver);



  static absl::optional<Fingerprint> FromStringToObj(const std::string& fingerprint_str);

  bool CanBeInitialized(){
    return !initialized_ && can_be_initialized_;
  }

  void SetCanBeInitialized(bool can_be){
    can_be_initialized_ = can_be;
  }
 private:
  friend class base::NoDestructor<FingerprintManager>;

  FingerprintManager();
  ~FingerprintManager() override;

  void LoadFingerprint(const std::string& fingerprint_file_path);

  base::Lock lock_;
  std::unique_ptr<Fingerprint> fingerprint_;

  std::string fingerprint_str_;

  bool initialized_ = false;

  bool loaded_ = false;

  // Usually set to false when switch isn't present
  bool can_be_initialized_ = true;


  mojo::ReceiverSet<fingerprinting::mojom::FingerprintManager> receivers_;

};

}  // namespace base

#endif  // FINGERPRINTING_MANAGER_H_