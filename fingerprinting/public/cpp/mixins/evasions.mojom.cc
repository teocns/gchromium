#include "fingerprinting/public/cpp/mixins/evasions.mojom.h"
#include <string>


namespace fingerprinting::internal {

void EvasionsMixinMojom::GetEvasions(
    mojom::HookTargetType target,
    mojom::FingerprintManager::GetEvasionsCallback callback) {


  // Compute cache key based on the target
  std::string cache_key = "GetEvasions_" + std::to_string(std::hash<mojom::HookTargetType>{}(target));

  // First, attempt to retrieve from cache.
  std::shared_ptr<evasions::Package> ptr;
  
  if (this->GetFromCache<evasions::Package>(cache_key, ptr)) {
    std::move(callback).Run(*ptr);
    return;
  }

  // If not in cache, compute the value and cache it.
  ptr = evasions::Package::Pack(target, _disable_evasions);
  this->CacheValue<evasions::Package>(cache_key, ptr);
  std::move(callback).Run(*ptr);
}

}  // namespace fingerprinting
