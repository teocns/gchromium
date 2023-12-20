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
  // core::evasions::Package* pack = nullptr;
  // if (this->cache.Get<core::evasions::Package>(cache_key, pack)) {
  //     std::move(callback).Run(std::move(pack));
  //     return;
  // }

  // Create the package and cache it
  auto nPack = core::evasions::Package::Pack(
      static_cast<core::evasions::HookTargetType>(target), GetFingerprint(),
      _disable_evasions);

  std::move(callback).Run(std::move(nPack));

  // this->cache.Set<core::evasions::Package>(cache_key, std::move(nPack));

  //
  // std::move(callback).Run(cpack);
  // Create the package and cache it
  // pack = core::evasions::Package::Pack(to_mojom(target), _disable_evasions);
  // this->cache.Value<mojom::EvasionsPack>(cache_key, ptr);
  // std::move(callback).Run(*ptr);
}

}  // namespace fingerprinting::internal
