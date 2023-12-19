// Copyright 2016 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FINGERPRINTING_EVASIONS_TRAITS_H
#define FINGERPRINTING_EVASIONS_TRAITS_H

#include "base/component_export.h"
#include "base/values.h"
// #include "fingerprinting/public/cpp/evasions/pack.h"
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
                 ::fingerprinting::core::evasions::Package> {
  static bool Read(::fingerprinting::mojom::EvasionsPackageDataView,
                   ::fingerprinting::core::evasions::Package*);

  static std::vector<::fingerprinting::core::evasions::Hook> hooks( const ::fingerprinting::core::evasions::Package& obj) {
    return obj.hooks;
  }

  static ::fingerprinting::core::evasions::HookTargetType target(const ::fingerprinting::core::evasions::Package& obj) {
    return obj.target;
    //
    // switch (type) {
    //   case ::fingerprinting::core::evasions::HookTargetType::PAGE:
    //     return fingerprinting::mojom::HookTargetType::PAGE;
    //   case ::fingerprinting::core::evasions::HookTargetType::WORKER:
    //     return fingerprinting::mojom::HookTargetType::WORKER;
    //   case ::fingerprinting::core::evasions::HookTargetType::SHARED_WORKER:
    //     return fingerprinting::mojom::HookTargetType::SHARED_WORKER;
    // }
    // NOTREACHED();
  }

  static base::Value fingerprint(const ::fingerprinting::core::evasions::Package&){
    return base::Value();
  }
};

// Hook
template <>
struct COMPONENT_EXPORT(EVASIONS_TRAITS)
    StructTraits<::fingerprinting::mojom::HookDataView,
                 ::fingerprinting::core::evasions::Hook> {
  static bool Read(::fingerprinting::mojom::HookDataView,
                   ::fingerprinting::core::evasions::Hook*);

  static std::string body(::fingerprinting::core::evasions::Hook& obj) {
    return obj.get_impl();
  }
  static std::string codename(::fingerprinting::core::evasions::Hook& obj) {
    return obj.codename;
  }
};

}  // namespace mojo

#endif  // FINGERPRINTING_EVASIONS_PACKAGE_TRAITS_H
