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
#include "fingerprinting/core/manager.h"
#include "fingerprinting/core/device_descriptor/fingerprint_impl.h"
#include "fingerprinting/export.h"
#include "fingerprinting/public/mojom/manager.mojom.h"
#include "fingerprinting/public/mojom/mixins/evasions.mojom.h"
#include "fingerprinting/public/mojom/mixins/user-agent.mojom.h"
#include "mojo/public/cpp/bindings/receiver.h"  // Add this line
#include "mojo/public/cpp/bindings/receiver_set.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

namespace fingerprinting {

class FINGERPRINTING_EXPORT FingerprintManager
    : virtual public FingerprintManagerCore,
      public mojom::FingerprintManager {
      // public UAMixinMojom,
      // public EvasionsMixinMojom {
 public:
  static FingerprintManager* GetInstance(bool try_init = false);

  void Bind(mojo::PendingReceiver<fingerprinting::mojom::FingerprintManager>
                receiver);

  void Enabled(EnabledCallback callback) override;

 private:
  FingerprintManager();

  ~FingerprintManager() override;
  friend class base::NoDestructor<FingerprintManager>;

  mojo::ReceiverSet<fingerprinting::mojom::FingerprintManager> receivers_;
};

FINGERPRINTING_EXPORT FingerprintManager* manager(bool try_init = false);

}  // namespace fingerprinting

#endif  // FINGERPRINTING_MANAGER_IMPL_H_
