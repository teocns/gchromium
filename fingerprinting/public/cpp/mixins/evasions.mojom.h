#ifndef FINGERPRINTING_EVASIONS_MIXIN_MOJOM_H
#define FINGERPRINTING_EVASIONS_MIXIN_MOJOM_H

#include "fingerprinting/core/evasions/pack.h"
#include "fingerprinting/public/cpp/common.h"
#include "fingerprinting/public/mojom/manager.mojom.h"

namespace fingerprinting::internal {

class COMPONENT_EXPORT(FINGERPRINTING_MANAGER) EvasionsMixinMojom
    : virtual public internal::FingerprintManagerBase {
 public:
  void GetEvasions(
      mojom::HookTargetType target,
      mojom::FingerprintManager::GetEvasionsCallback callback) override;
 private:
  // TODO: Evasions codenames to disable, passed by --disable-evasions launch
  // flag
  std::set<std::string> _disable_evasions;
};

}  // namespace fingerprinting::internal

namespace mojo{

// template<> struct mojo::EnumTraits<::fingerprinting::mojom::EvasionsPack, ::fingerprinting::core::evasions::Package> {
//
//   static ::fingerprinting::mojom::EvasionsPack ToMojom(::fingerprinting::core::evasions::Package package) {
//     return ::fingerprinting::mojom::EvasionsPack(
//         package.target,
//         mojo::ConvertTo<std::map<std::string, std::string>>(package.hooks)
//     );
//   }
// };


//
// template <>
// struct EnumTraits<fingerprinting::mojom::HookTargetType, fingerprinting::mojom::HookTargetType> {
//   static fingerprinting::mojom::HookTargetType ToMojom(fingerprinting::mojom::HookTargetType input) {
//     return input;
//   }
//
//   static bool FromMojom(fingerprinting::mojom::HookTargetType input, fingerprinting::mojom::HookTargetType* output) {
//     *output = input;
//     return true;
//   }
// };
//
// template <>
// struct StructTraits<fingerprinting::mojom::EvasionsPackDataView, fingerprinting::mojom::EvasionsPack> {
//   static fingerprinting::mojom::HookTargetType target(const fingerprinting::mojom::EvasionsPack& pack) {
//     return pack.target;
//   }
//
//   static const mojo_base::mojom::Value& hooks(const fingerprinting::mojom::EvasionsPack& pack) {
//     return pack.hooks;
//   }
//
//   static bool Read(fingerprinting::mojom::EvasionsPackDataView data_view, fingerprinting::mojom::EvasionsPack* out);
// };

}  // namespace mojo



#endif  // FINGERPRINTING_EVASIONS_MIXIN_MOJOM_H



