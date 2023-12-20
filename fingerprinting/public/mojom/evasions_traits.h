// Copyright 2016 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FINGERPRINTING_EVASIONS_TRAITS_H
#define FINGERPRINTING_EVASIONS_TRAITS_H

#include "base/component_export.h"
#include "base/values.h"
#include "fingerprinting/core/evasions/hook.h"
#include "fingerprinting/core/evasions/pack.h"
#include "fingerprinting/public/mojom/evasions.mojom-shared.h"

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

// EvasionsPackage
template <>
struct COMPONENT_EXPORT(EVASIONS_TRAITS)
    StructTraits<::fingerprinting::mojom::EvasionsPackageDataView,
                 std::unique_ptr<::fingerprinting::core::evasions::Package>> {
  static bool Read(::fingerprinting::mojom::EvasionsPackageDataView,
                   std::unique_ptr<::fingerprinting::core::evasions::Package>*);

  static const std::vector<
      std::unique_ptr<::fingerprinting::core::evasions::Hook>>

  hooks(const std::unique_ptr<::fingerprinting::core::evasions::Package>& obj) {
    return std::vector<
        std::unique_ptr<::fingerprinting::core::evasions::Hook>>();
  }

  static::fingerprinting::core::evasions::HookTargetType target( const std::unique_ptr<::fingerprinting::core::evasions::Package>& obj) {
    return obj->target;
  }

  static base::Value fingerprint(
const std::unique_ptr<::fingerprinting::core::evasions::Package>& obj) {
    return base::Value();
  }
};

// Hook
template <>
struct COMPONENT_EXPORT(EVASIONS_TRAITS)
    StructTraits<::fingerprinting::mojom::HookDataView,
                 std::unique_ptr<::fingerprinting::core::evasions::Hook>> {
  static bool Read(::fingerprinting::mojom::HookDataView,
                   std::unique_ptr<::fingerprinting::core::evasions::Hook>*);

  static std::string codename(
      const std::unique_ptr<::fingerprinting::core::evasions::Hook>& obj) {
    return obj->codename();
  }
};

}  // namespace mojo

#endif  // FINGERPRINTING_EVASIONS_PACKAGE_TRAITS_H
