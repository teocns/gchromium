// Copyright 2015 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FINGERPRINTING_USER_AGENT_UTILITY_H
#define FINGERPRINTING_USER_AGENT_UTILITY_H

#include <map>
#include <regex>
#include <string>
#include "base/values.h"

#include "base/component_export.h"

namespace blink {
struct UserAgentMetadata;
}

namespace fingerprinting::utility::user_agent {

// bool PickleUserAgentMetadata(base::Value::Dict& in, std::string& out);
COMPONENT_EXPORT(FINGERPRINTING_UTILITY_UA)
bool MakeUserAgentMetadata(base::Value::Dict& in,
                           blink::UserAgentMetadata& out);

}  // namespace fingerprinting
#endif  // FINGERPRINTING_USER_AGENT_UTILITY_H
