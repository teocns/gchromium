// Copyright 2016 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FINGERPRINTING_EVASIONS_TRAITS_H
#define FINGERPRINTING_EVASIONS_TRAITS_H

#include "base/component_export.h"
#include "base/values.h"
#include "fingerprinting/core/device_descriptor/fingerprint_impl.h"
#include "fingerprinting/core/evasions/hook.h"
#include "fingerprinting/core/evasions/pack.h"
#include "fingerprinting/public/mojom/evasions.mojom-shared.h"
#include "third_party/eigen3/src/Eigen/src/Core/util/XprHelper.h"

namespace mojo {

// HookTargetType
template <>
struct COMPONENT_EXPORT(EVASIONS_TRAITS)
    EnumTraits<::fingerprinting::mojom::HookTargetType,
               ::fingerprinting::core::evasions::HookTargetType> {
  static ::fingerprinting::mojom::HookTargetType ToMojom(
      ::fingerprinting::core::evasions::HookTargetType type);

  static bool FromMojom(::fingerprinting::mojom::HookTargetType type,
                        ::fingerprinting::core::evasions::HookTargetType* out);
};

EvasionsPackage
template <>
struct COMPONENT_EXPORT(EVASIONS_TRAITS) StructTraits<
    ::fingerprinting::mojom::EvasionsPackageDataView,
    std::shared_ptr<::fingerprinting::core::evasions::EvasionsPackage>> {
  static bool Read(
      ::fingerprinting::mojom::EvasionsPackageDataView,
      std::shared_ptr<::fingerprinting::core::evasions::EvasionsPackage>*);

  static const std::vector<
      std::unique_ptr<::fingerprinting::core::evasions::Hook>>

  hooks(
      const std::shared_ptr<::fingerprinting::core::evasions::EvasionsPackage>&
          obj) {
    std::vector<std::unique_ptr<::fingerprinting::core::evasions::Hook>> ret;
    for (auto& hook : obj->hooks) {
      ret.push_back(
          std::unique_ptr<::fingerprinting::core::evasions::Hook>(hook.get()));
    }
    return ret;
  }

  static ::fingerprinting::core::evasions::HookTargetType target(
      const std::shared_ptr<::fingerprinting::core::evasions::EvasionsPackage>&
          obj) {
    return obj->target;
  }

  static std::string fingerprint(
      const std::shared_ptr<::fingerprinting::core::evasions::EvasionsPackage>&
          obj) {
      return obj->fingerprint;
  }
};

// Hook
template <>
struct COMPONENT_EXPORT(EVASIONS_TRAITS)
    StructTraits<::fingerprinting::mojom::FingerprintDataView,
                 std::unique_ptr<::fingerprinting::Fingerprint>> {
  static bool Read(::fingerprinting::mojom::FingerprintDataView,
                   std::unique_ptr<::fingerprinting::core::evasions::Hook>*);

  //value 
  static base::Value value(
      const std::unique_ptr<::fingerprinting::Fingerprint>&
          obj) {
    return obj->value().Clone();
  }
};

}  // namespace mojo

#endif  // FINGERPRINTING_EVASIONS_PACKAGE_TRAITS_H
