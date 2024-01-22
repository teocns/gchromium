// fingerprint_manager.h
#ifndef COMPONENTS_FINGERPRINTING_MANAGER_IMPL_H_
#define COMPONENTS_FINGERPRINTING_MANAGER_IMPL_H_

#include <string>

#include "base/component_export.h"
#include "base/files/file_util.h"
#include "base/json/json_reader.h"
#include "base/logging.h"
#include "base/no_destructor.h"
#include "base/strings/string_util.h"

#include "base/values.h"
#include "components/fingerprinting/common/fingerprint_impl.h"
#include "components/fingerprinting/common/manager.h"
#include "components/fingerprinting/common/mojom/manager.mojom.h"

#include "mojo/public/cpp/bindings/receiver.h"  // Add this line
#include "mojo/public/cpp/bindings/receiver_set.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

namespace fingerprinting {

class COMPONENT_EXPORT(COMPONENTS_FINGERPRINTING_BROWSER_IMPL)
    FingerprintManagerImpl
    : virtual public fingerprinting::FingerprintManager,
      virtual public fingerprinting::mojom::FingerprintManager {
 public:
  static FingerprintManagerImpl* GetInstance(bool try_init = false);

  void AddReceiver(
      mojo::PendingReceiver<fingerprinting::mojom::FingerprintManager>
          receiver);

  void Enabled(EnabledCallback callback) override;
  void GetFingerprintString(GetFingerprintStringCallback) override;
  void GetFingerprintValue(GetFingerprintValueCallback) override;

  void MaybeInitialize();

 private:
  FingerprintManagerImpl();

  // ~FingerprintManagerImpl() override;
  friend class base::NoDestructor<FingerprintManagerImpl>;

  mojo::ReceiverSet<fingerprinting::mojom::FingerprintManager> receivers_;
};

}  // namespace fingerprinting

#endif  // COMPONENTS_FINGERPRINTING_MANAGER_IMPL_H_
