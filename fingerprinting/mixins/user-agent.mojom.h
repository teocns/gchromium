#ifndef FINGERPRINTING_MOJOM_UAMIXIN_H
#define FINGERPRINTING_MOJOM_UAMIXIN_H


#include "fingerprinting/export.h"
#include "fingerprinting/manager.h"
#include "third_party/abseil-cpp/absl/types/optional.h"
namespace blink{
  struct UserAgentMetadata;
}

namespace fingerprinting {



class SetFingerprintStrCallback;
class GetFingerprintStrCallback;
class GetUserAgentClientHintsCallback;






class FINGERPRINTING_EXPORT UAMixinMojom : virtual public IFingerprintManager{
public:

  void GetUserAgentClientHints(GetUserAgentClientHintsCallback callback) override;
  void GetUserAgentMetadata(GetUserAgentMetadataCallback callback) override;
  absl::optional<blink::UserAgentMetadata> GetUserAgentMetadata_();
  absl::optional<std::string> GetUserAgent_();
};

}  // namespace fingerprinting

#endif  // FINGERPRINTING_MOJOM_UAMIXIN_H
