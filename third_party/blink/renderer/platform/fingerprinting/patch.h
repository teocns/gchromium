// Copyright 2017 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FINGERPRINTING_PLATFORM_CONTROLLER
#define FINGERPRINTING_PLATFORM_CONTROLLER

#include "base/values.h"
#include "fingerprinting/core/device_descriptor/fingerprint_impl.h"
#include "third_party/blink/renderer/platform/platform_export.h"

namespace blink {

// This object is a process-wide singleton, and thread-safe.
class PLATFORM_EXPORT FingerprintingResourceController {
 public:
  static void Set(fingerprinting::Fingerprint*);

  static fingerprinting::Fingerprint* Get();
};
}  // namespace blink

#endif  // FINGERPRINTING_PLATFORM_CONTROLLER
