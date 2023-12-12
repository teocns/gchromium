#ifndef FINGERPRINTING_UA_MIXIN_MOJOM_H
#define FINGERPRINTING_UA_MIXIN_MOJOM_H

#include "fingerprinting/core/manager.h"
#include "fingerprinting/public/cpp/common.h"
#include "fingerprinting/public/cpp/export.h"
#include "third_party/abseil-cpp/absl/types/optional.h"
namespace blink {
struct UserAgentMetadata;
}

namespace fingerprinting::internal {

class SetFingerprintStrCallback;
class GetFingerprintStrCallback;
class GetUserAgentClientHintsCallback;

class FINGERPRINTING_PUBLIC_EXPORT UAMixinMojom
    : virtual public internal::FingerprintManagerBase {
 public:
  void GetUserAgentClientHints(
      GetUserAgentClientHintsCallback callback) override;
  void GetUserAgentMetadata(GetUserAgentMetadataCallback callback) override;
  absl::optional<blink::UserAgentMetadata> GetUserAgentMetadata_();
  absl::optional<std::string> GetUserAgent_();
};

}  // namespace fingerprinting::internal

#endif  // FINGERPRINTING_UA_MIXIN_MOJOM_H
