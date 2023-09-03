// Copyright 2015 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FINGERPRINTING_USER_AGENT_UTILITIES_H
#define FINGERPRINTING_USER_AGENT_UTILITIES_H

#include <map>
#include <regex>
#include <string>
#include "base/values.h"
#include "fingerprinting/utilities/export.h"



namespace blink{
  struct UserAgentMetadata;
}


namespace fingerprinting {

namespace utilities {
namespace user_agent {




// bool PickleUserAgentMetadata(base::Value::Dict& in, std::string& out);
FINGERPRINTING_UTILITY_EXPORT bool MakeUserAgentMetadata(base::Value::Dict& in, blink::UserAgentMetadata& out);


}  // namespace user_agent
}  // namespace utilities
}  // namespace fingerprinting
#endif  // FINGERPRINTING_USER_AGENT_UTILITIES_H
