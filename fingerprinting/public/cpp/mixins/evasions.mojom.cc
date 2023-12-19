#include "fingerprinting/public/cpp/mixins/evasions.mojom.h"
#include <string>
#include "fingerprinting/public/mojom/manager.mojom.h"

namespace fingerprinting::internal {

void EvasionsMixinMojom::GetEvasions(
    core::evasions::HookTargetType target,
    mojom::FingerprintManager::GetEvasionsCallback callback) {

  // Compute cache key based on the target
  std::string cache_key =
      "GetEvasions_" +
      std::to_string(std::hash<core::evasions::HookTargetType>{}(target));

  // First, attempt to retrieve from cache.
  absl::optional<core::evasions::Package> pack;

  if (!this->cache.Get<core::evasions::Package>(cache_key, pack)) {
    // Create the package and cache it
    pack = core::evasions::Package::Pack(
        static_cast<core::evasions::HookTargetType>(target), _disable_evasions);

    this->cache.Set<core::evasions::Package>(cache_key, pack.value());
  }
  
  std::move(callback).Run(pack.value());
  //
  // std::move(callback).Run(cpack);
  // Create the package and cache it
  // pack = core::evasions::Package::Pack(to_mojom(target), _disable_evasions);
  // this->cache.Value<mojom::EvasionsPack>(cache_key, ptr);
  // std::move(callback).Run(*ptr);
}

}  // namespace fingerprinting::internal
