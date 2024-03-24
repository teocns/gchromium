// fingerprint_manager.h
#ifndef FINGERPRINTING_MANAGER_IMPL_H_
#define FINGERPRINTING_MANAGER_IMPL_H_

#include <string>

#include "base/files/file_util.h"
#include "base/json/json_reader.h"
#include "base/logging.h"
#include "base/no_destructor.h"
#include "base/strings/string_util.h"

#include "base/values.h"
#include "fingerprinting/core/device_descriptor/fingerprint_impl.h"
#include "fingerprinting/core/manager.h"
#include "fingerprinting/public/cpp/export.h"
// #include "fingerprinting/public/cpp/mixins/evasions.mojom.h"
#include "fingerprinting/public/cpp/mixins/user-agent.mojom.h"
#include "fingerprinting/public/mojom/manager.mojom.h"

#include "mojo/public/cpp/bindings/receiver.h"  // Add this line
#include "mojo/public/cpp/bindings/receiver_set.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

#include "fingerprinting/public/cpp/common.h"
namespace fingerprinting {

class FINGERPRINTING_PUBLIC_EXPORT FingerprintManager
    : virtual public internal::FingerprintManagerBase,
      public internal::UAMixinMojom {
 public:
  static FingerprintManager* GetInstance(bool try_init = false);

  void Bind(mojo::PendingReceiver<fingerprinting::mojom::FingerprintManager>
                receiver);

  void Enabled(EnabledCallback callback) override;
  void GetFingerprintString(GetFingerprintStringCallback) override;
  void GetFingerprintValue(GetFingerprintValueCallback) override;

 private:
  FingerprintManager();

  ~FingerprintManager() override;
  friend class base::NoDestructor<FingerprintManager>;

  mojo::ReceiverSet<fingerprinting::mojom::FingerprintManager> receivers_;
};

FINGERPRINTING_PUBLIC_EXPORT FingerprintManager* manager(bool try_init = false);

}  // namespace fingerprinting

#endif  // FINGERPRINTING_MANAGER_IMPL_H_
