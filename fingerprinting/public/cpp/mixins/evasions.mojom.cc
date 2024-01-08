#include "fingerprinting/public/cpp/mixins/evasions.mojom.h"
#include <memory>
#include <string>
#include "fingerprinting/public/mojom/manager.mojom.h"

namespace fingerprinting::internal {

void EvasionsMixinMojom::GetEvasions(
    const std::string& target,
    mojom::FingerprintManager::GetEvasionsCallback callback) {
  // Compute cache key based on the target
  // std::string cache_key =
  //     "GetEvasions_" +
  //     std::to_string(std::hash<core::evasions::HookTargetType>{}(target));
  //
  // First, attempt to retrieve from cache.

  // std::shared_ptr<core::evasions::EvasionsPackage> sptr = nullptr;
  // if (!this->cache.Get<core::evasions::EvasionsPackage>(cache_key, sptr)) {
    // sptr = core::evasions::EvasionsPackage::Pack(
    //     static_cast<core::evasions::HookTargetType>(target), GetFingerprint(),
    //     _disable_evasions);
  //
  //   // Turn into a shared_ptr and cache it
  //   this->cache.Set<core::evasions::EvasionsPackage>(cache_key, sptr);
  // }


  // std::move(callback).Run(sptr);
  // Create the package and cache it

  //
  // std::move(callback).Run(cpack);
  // Create the package and cache it
  // pack = core::evasions::EvasionsPackage::Pack(to_mojom(target),
  // _disable_evasions); this->cache.Value<mojom::EvasionsPack>(cache_key, ptr);
  // std::move(callback).Run(*ptr);



  std::move(callback).Run(GetFingerprint()->value().Clone());
}

}  // namespace fingerprinting::internal
