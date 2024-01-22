#ifndef COMPONENT_FINGERPRINTING_HELPER_USER_AGENT_H
#define COMPONENT_FINGERPRINTING_HELPER_USER_AGENT_H

#include <map>
#include <regex>
#include <string>
#include "base/values.h"

#include "base/component_export.h"

namespace blink {
struct UserAgentMetadata;
}

namespace fingerprinting::helper {

// bool PickleUserAgentMetadata(base::Value::Dict& in, std::string& out);
bool MakeUserAgentMetadata(base::Value::Dict& in,
                           blink::UserAgentMetadata& out);

}  // namespace fingerprinting::utility
#endif  // COMPONENT_FINGERPRINTING_HELPER_USER_AGENT_H

