// Copyright 2015 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "fingerprinting/utilities/user_agent.h"
#include <map>
#include <regex>
#include <string>
#include "base/logging.h"
#include "base/pickle.h"
#include "base/values.h"
#include "third_party/abseil-cpp/absl/types/optional.h"
#include "third_party/blink/public/common/user_agent/user_agent_metadata.h"

namespace fingerprinting {
namespace utilities {
namespace user_agent {
namespace {

/**
* UACH Hints:


architecture
bitness
model
platformVersion
uaFullVersion
fullVersionList

"highEntropyValues": {
    "brands": [
      {
        "brand": "Chromium",
        "version": "115"
      },
      {
        "brand": "Not)A;Brand",
        "version": "24"
      },
      {
        "brand": "Google Chrome",
        "version": "115"
      }
    ],
    "mobile": false,
    "bitness": "64",
    "platform": "macOS",
    "architecture": "arm",

    "platformVersion": "13.3.1"
  }

*
*/

// constexpr uint32_t kPickleVersion =
//     2u;  // Must reflect the same value as in
//          // third_party/blink/common/user_agent/user_agent_metadata.cc

constexpr char kMobileFormFactor[] =
    "Mobile";  // Must reflect the same value as in
               // components/embedder_support/user_agent_utils.h

constexpr char kFullVersion[] = "uaFullVersion";  // Might be depreacted
constexpr char kPlatform[] = "platform";
constexpr char kPlatformVersion[] = "platformVersion";
constexpr char kArchitecture[] = "architecture";
constexpr char kModel[] = "model";
constexpr char kMobile[] = "mobile";
constexpr char kBitness[] = "bitness";
// constexpr char kWow64[] = "wow64";
constexpr char kBrands[] = "brands";
constexpr char kFullVersionList[] = "fullVersionList";

constexpr char kBrand[] = "brand";
constexpr char kVersion[] = "version";

//  "Android", "Chrome OS", "Fuchsia", "iOS", "Linux", "macOS", "Windows", or
//  "Unknown".
// constexpr char kPlatformAndroid[] = "Android";
// constexpr char kPlatformMacOS[] = "macOS";
// constexpr char kPlatformWindows[] = "Windows";
// constexpr char kPlatformLinux[] = "Linux";
// constexpr char kPlatformIOS[] = "iOS";
// constexpr char kPlatformChromeOS[] = "Chrome OS";
// constexpr char kPlatformFuchsia[] = "Fuchsia";
// constexpr char kPlatformUnknown[] = "Unknown";

// constexpr char kBitness64[] = "64";
// constexpr char kBitness32[] = "32";
// constexpr char kArchitectureArm[] = "arm";
// constexpr char kArchitectureX86[] = "x86";

// std::string GetRealBrandAndVersion(const base::Value::List& brands) {
//   for (const auto& brand : brands) {
//     const auto* bv = brand.GetIfDict();
//     if (!bv) continue;
//     auto brand_name = bv->FindString("brand"), version =
//     bv->FindString("version"); if (brand_name && version && *brand_name ==
//     "Chromium") return *version;
//   }
// }

// const std::string ParsePlatform(){

// }

// bool IsWow64(const base::Value::Dict* highEntropyValues, bool& out) const {
//     /**
//      * Determines whether the UserAgentMetadata is a WOW64
//      *
//      * Source: src/base/win/windows_version.h
//      * WowProcessMachine::kX86 && WowNativeMachine::kAMD64
//      *
//      * Returns success if the operation succeeded
//      * The isWow64 real value is written to out
//     */
//     const std::string* bitness = highEntropyValues->FindString(kBitness);

//     const std::string* architecture =
//     highEntropyValues->FindString(kArchitecture);

//     if (bitness && architecture) {
//         out = (*bitness == kBitness32 && *architecture == "amd64");
//         return true;
//     }
//     return false;
// }

// bool PickleUserAgentMetadata(const base::Value::Dict& highEntropyValues,
// std::string& out) const {
//   /**
//    * Returns a string that is serializable with
//    UserAgentMetadata::Demarshal()
//    * This method re-creates / mirrors the UserAgentMetadata::Marshal() method
//    * Source: third_party/blink/common/user_agent/user_agent_metadata.cc
//    */

//     base::Pickle metadata;

//     // Brands size
//     metadata.WriteUInt32(kPickleVersion);

//     // brand_version_list
//     const base::Value::List* brand_version_list =
//         highEntropyValues.FindList("brands");

//     if (!brand_version_list || !(brand_version_list->size() > 0)) {
//       return false;
//     }

//     metadata.WriteUInt32(brand_version_list->size());

//     for (size_t i = 0; i < brand_version_list->size(); ++i) {
//       const base::Value* brand_version = &(*brand_version_list)[i];

//       const base::Value::Dict* bv = brand_version->GetIfDict();
//       if (!bv) {
//         continue;
//       }

//       const std::string* brand = bv->FindString("brand");
//       const std::string* version = bv->FindString("version");

//       if (!(brand && version)) {
//         return false;
//       }

//       metadata.WriteString(*brand);
//       metadata.WriteString(*version);
//     }
//     // brand_full_version_list
//     const base::Value::List* brand_full_version_list =
//         highEntropyValues.FindList("fullVersionList");

//     if (!brand_full_version_list || !(brand_full_version_list->size() > 0)) {
//       return false;
//     }

//     metadata.WriteUInt32(brand_full_version_list->size());

//     for (size_t i = 0; i < brand_full_version_list->size(); ++i) {
//       const base::Value* brand_version = &(*brand_full_version_list)[i];

//       const base::Value::Dict* bv = brand_version->GetIfDict();
//       if (!bv) {
//         continue;
//       }

//       const std::string* brand = bv->FindString(kBrand);
//       const std::string* version = bv->FindString("version");

//       if (!(brand && version)) {
//         return false;
//       }

//       metadata.WriteString(*brand);
//       metadata.WriteString(*version);
//     }

//     const std::string* full_version =
//     highEntropyValues.FindString(kFullVersion); const std::string* platform =
//     highEntropyValues.FindString(kPlatform); const std::string*
//     platform_version = highEntropyValues.FindString(kPlatformVersion); const
//     std::string* architecture = highEntropyValues.FindString(kArchitecture);
//     const std::string* model = highEntropyValues.FindString(kModel);
//     absl::optional<bool> mobile = highEntropyValues.FindBool(kMobile);
//     bool is_mobile = mobile.value_or(false);
//     const std::string* bitness = highEntropyValues.FindString(kBitness);
//     absl::optional<bool> wow64 = highEntropyValues.FindBool(kWow64);

//     if (!full_version) {
//       return false;
//     }
//     metadata.WriteString(*full_version);

//     if (!platform) {
//       return false;
//     }
//     metadata.WriteString(*platform);

//     if (!platform_version) {
//       return false;
//     }
//     metadata.WriteString(*platform_version);

//     if (!architecture) {
//       return false;
//     }
//     metadata.WriteString(*architecture);

//     if (!model) {
//       return false;
//     }
//     metadata.WriteString(*model);

//     metadata.WriteBool(is_mobile);

//     if (!bitness) {
//       return false;
//     }
//     metadata.WriteString(*bitness);

//     // wow64
//     metadata.WriteBool(wow64.value_or(false));

//     metadata.WriteString(is_mobile ? kMobileFormFactor : "");

//     out = std::string(reinterpret_cast<const char*>(metadata.data()),
//                       metadata.size());

//     return true;
// }

bool populate_brands_list(blink::UserAgentBrandList& out,
                          base::Value::List* brands_list) {
  /**
   * Iterates over raw brands list and populates the UserAgentBrandList
   * Returns false if the operation failed
   */
  if (!brands_list || !(brands_list->size() > 0)) {
    return false;
  }
  for (size_t i = 0; i < brands_list->size(); ++i) {
    const base::Value* brand_version = &(*brands_list)[i];

    const base::Value::Dict* bv = brand_version->GetIfDict();
    if (!bv) {
      return false;
    }

    const std::string* brand = bv->FindString(kBrand);
    const std::string* version = bv->FindString(kVersion);

    if (!(brand && version)) {
      return false;
    }
    out.push_back(blink::UserAgentBrandVersion(*brand, *version));
  }

  return true;
}

}  // Anonymous namespace

bool MakeUserAgentMetadata(base::Value::Dict& in,
                           blink::UserAgentMetadata& out) {
  if (!populate_brands_list(out.brand_version_list, in.FindList(kBrands))) {
    LOG(ERROR) << "Failed parsing UA Metadata: brand_version_list invalid.";
    return false;
  }

  if (!populate_brands_list(out.brand_full_version_list,
                            in.FindList(kFullVersionList))) {
    LOG(ERROR)
        << "Failed parsing UA Metadata: brand_full_version_list invalid.";
    return false;
  }

  const std::string* full_version = in.FindString(kFullVersion);
  const std::string* platform = in.FindString(kPlatform);
  const std::string* platform_version = in.FindString(kPlatformVersion);
  const std::string* architecture = in.FindString(kArchitecture);
  const std::string* model = in.FindString(kModel);
  absl::optional<bool> mobile = in.FindBool(kMobile);
  bool is_mobile = mobile.value_or(false);
  const std::string* bitness = in.FindString(kBitness);
  // absl::optional<bool> wow64 = highEntropyValues.FindBool(kWow64)

  // Deprecated. Don't fail if not found
  if (full_version != nullptr) {
    out.full_version = *full_version;
  }

  if (platform == nullptr) {
    LOG(ERROR) << "Failed parsing UA Metadata: platform field not set.";
    return false;
  }
  out.platform = *platform;

  if (platform_version == nullptr) {
    LOG(ERROR) << "Failed parsing UA Metadata: platform_version field not set.";
    return false;
  }
  out.platform_version = *platform_version;

  if (architecture == nullptr) {
    LOG(ERROR) << "Failed parsing UA Metadata: architecture field not set.";
    return false;
  }
  out.architecture = *architecture;

  if (model != nullptr) {
    out.model = *model;
  }

  out.mobile = is_mobile;

  if (bitness == nullptr) {
    LOG(ERROR) << "Failed parsing UA Metadata: bitness field not set.";
    return false;
  }
  out.bitness = *bitness;

  // wow64
  // out.WriteBool(wow64.value_or(false));;
  out.wow64 = false;

  out.form_factor = is_mobile ? kMobileFormFactor : "";

  return true;
}

}  // namespace user_agent
}  // namespace utilities
}  // namespace fingerprinting
