// Copyright 2021 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BLINK_EXTENSIONS_GHOST_FINGERPRINTING
#define BLINK_EXTENSIONS_GHOST_FINGERPRINTING


#include "base/component_export.h"
namespace blink {

class COMPONENT_EXPORT(BLINK_EXTENSIONS_GHOST) FingerprintingExtensions {
 public:
  static void Initialize();
};

}  // namespace blink

#endif  // BLINK_EXTENSIONS_GHOST_FINGERPRINTING
