#ifndef FINGERPRINTING_UA_MIXIN_MOJOM_H
#define FINGERPRINTING_UA_MIXIN_MOJOM_H


#include "base/component_export.h"
#include "fingerprinting/public/cpp/common.h"
#include "fingerprinting/core/manager.h"
#include "third_party/abseil-cpp/absl/types/optional.h"
namespace blink{
  struct UserAgentMetadata;
}

namespace fingerprinting::internal {



class SetFingerprintStrCallback;
class GetFingerprintStrCallback;
class GetUserAgentClientHintsCallback;


class COMPONENT_EXPORT(FINGERPRINTING_MANAGER) UAMixinMojom : virtual public internal::FingerprintManagerBase{
public:

  void GetUserAgentClientHints(GetUserAgentClientHintsCallback callback) override;
  void GetUserAgentMetadata(GetUserAgentMetadataCallback callback) override;
  absl::optional<blink::UserAgentMetadata> GetUserAgentMetadata_();
  absl::optional<std::string> GetUserAgent_();
};

}  // namespace fingerprinting

#endif  // FINGERPRINTING_UA_MIXIN_MOJOM_H
