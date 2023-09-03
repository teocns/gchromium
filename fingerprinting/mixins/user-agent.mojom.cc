

#include "fingerprinting/mixins/user-agent.mojom.h"
#include "base/logging.h"
#include "third_party/blink/public/common/user_agent/user_agent_metadata.h"

// Include absl
#include "third_party/abseil-cpp/absl/types/optional.h"

namespace fingerprinting {

namespace {
constexpr char kUserAgentMetadata[] = "user_agent_metadata";
constexpr char kUserAgent[] = "user_agent";
}  // namespace

void UAMixinMojom::GetUserAgentClientHints(
    GetUserAgentClientHintsCallback callback) {
  base::Value* out = nullptr;

  if (GetFingerprint()->GetUserAgentClientHints(out)) {
    base::Value& value = *out;
    base::Value clone(value.Clone());

    std::move(callback).Run(std::move(clone));
  } else {
    std::move(callback).Run(base::Value());
  }
}

absl::optional<blink::UserAgentMetadata> UAMixinMojom::GetUserAgentMetadata_() {
  blink::UserAgentMetadata out;
  /**
   * Non-mojom version, same-process callable
   */
  if (this->GetFromCache<blink::UserAgentMetadata>(kUserAgentMetadata, out)) {
    return absl::optional<blink::UserAgentMetadata>(std::move(out));
  }

  if (GetFingerprint()->GetUserAgentMetadata(out)) {
    this->CacheValue<blink::UserAgentMetadata>(kUserAgentMetadata, out);
    DLOG(WARNING)
        << "Computing same-process GetUserAgentMetadata without cache";
    return absl::optional<blink::UserAgentMetadata>(std::move(out));
  }
  return absl::nullopt;
}

void UAMixinMojom::GetUserAgentMetadata(GetUserAgentMetadataCallback callback) {
  absl::optional<blink::UserAgentMetadata> out = this->GetUserAgentMetadata_();
  if (out) {
    std::move(callback).Run(out);
  } else {
    std::move(callback).Run(absl::nullopt);
  }
}

absl::optional<std::string> UAMixinMojom::GetUserAgent_() {
  std::string out;
  if (this->GetFromCache<std::string>(kUserAgent, out)) {
    return absl::optional<std::string>(std::move(out));
  }
  if (GetFingerprint()->GetUserAgent(out)) {
    this->CacheValue<std::string>(kUserAgent, out);
    return absl::optional<std::string>(std::move(out));
  } else {
    return absl::nullopt;
  }
}


}  // namespace fingerprinting
