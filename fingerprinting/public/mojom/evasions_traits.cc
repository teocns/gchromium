#include "fingerprinting/public/mojom/evasions_traits.h"
#include "fingerprinting/core/evasions/hook.h"
#include "fingerprinting/public/mojom/evasions.mojom-shared.h"
#include "mojo/public/cpp/bindings/struct_traits.h"

// Forward declaration of the `Package` structure, if not already included
// namespace ::fingerprinting::core::evasions {
// struct Package;
// }

// Mojo traits specialization for the `Package` structure
namespace mojo {

bool StructTraits<::fingerprinting::mojom::EvasionsPackageDataView,
                  ::fingerprinting::core::evasions::Package>::
    Read(::fingerprinting::mojom::EvasionsPackageDataView data_view,
         ::fingerprinting::core::evasions::Package* out) {
  // The deserialization logic would be here, for example:
  if (!data_view.ReadHooks(&out->hooks)) {
    return false;
  }

  if (!data_view.ReadTarget(&out->target)) {
    return false;
  }

  return true;
}

bool StructTraits<::fingerprinting::mojom::HookDataView,
                  ::fingerprinting::core::evasions::Hook>::
    Read(::fingerprinting::mojom::HookDataView data_view,
         ::fingerprinting::core::evasions::Hook* out) {
  std::string body;
  if (!data_view.ReadBody(&body)) {
    return false;
  }

  std::string codename;
  if (!data_view.ReadCodename(&codename)) {
    return false;
  }

  *out = ::fingerprinting::core::evasions::Hook(body, codename);

  return true;
}

// static ::fingerprinting::mojom::HookTargetType ToMojom(
//     ::fingerprinting::core::evasions::HookTargetType type);
//
// static bool FromMojom(::fingerprinting::mojom::HookTargetType type,
//                       ::fingerprinting::core::evasions::HookTargetType* out);

bool EnumTraits<::fingerprinting::mojom::HookTargetType,
                ::fingerprinting::core::evasions::HookTargetType>::
    FromMojom(fingerprinting::mojom::HookTargetType type,
              ::fingerprinting::core::evasions::HookTargetType* out) {
  switch (type) {
    case fingerprinting::mojom::HookTargetType::PAGE:
      *out = ::fingerprinting::core::evasions::HookTargetType::PAGE;
      return true;
    case fingerprinting::mojom::HookTargetType::WORKER:
      *out = ::fingerprinting::core::evasions::HookTargetType::WORKER;
      return true;
    case fingerprinting::mojom::HookTargetType::SHARED_WORKER:
      *out = ::fingerprinting::core::evasions::HookTargetType::SHARED_WORKER;
      return true;
  }
  return false;
}

// ToMojom
::fingerprinting::mojom::HookTargetType
EnumTraits<::fingerprinting::mojom::HookTargetType,
           ::fingerprinting::core::evasions::HookTargetType>::
    ToMojom(::fingerprinting::core::evasions::HookTargetType type) {
  switch (type) {
    case ::fingerprinting::core::evasions::HookTargetType::PAGE:
      return fingerprinting::mojom::HookTargetType::PAGE;
    case ::fingerprinting::core::evasions::HookTargetType::WORKER:
      return fingerprinting::mojom::HookTargetType::WORKER;
    case ::fingerprinting::core::evasions::HookTargetType::SHARED_WORKER:
      return fingerprinting::mojom::HookTargetType::SHARED_WORKER;
  }
  NOTREACHED();
}
}  // namespace mojo
