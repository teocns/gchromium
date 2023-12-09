#ifndef FINGERPRINTING_UA_MIXIN_H_
#define FINGERPRINTING_UA_MIXIN_H_

#include "base/values.h"
#include "fingerprinting/export.h"
#include "fingerprinting/fingerprint.h"
#include "fingerprinting/utilities/user_agent.h"


namespace blink{
  struct UserAgentMetadata;
}


namespace fingerprinting {

namespace {
const uint32_t kVersion =
    2u;  // Must reflect the same value as in
         // third_party/blink/common/user_agent/user_agent_metadata.cc
}


class FINGERPRINTING_EXPORT UAMixin : protected virtual IFingerprint {
  /**
   "highEntropyValues": {
"architecture": "arm",
"bitness": "64",
"brands": [
{ "brand": "Chromium", "version": "116" },
{ "brand": "Not)A;Brand", "version": "24" },
{ "brand": "Google Chrome", "version": "116" }
],
"fullVersionList": [
{ "brand": "Chromium", "version": "116.0.5845.96" },
{ "brand": "Not)A;Brand", "version": "24.0.0.0" },
{ "brand": "Google Chrome", "version": "116.0.5845.96" }
],
"mobile": false,
"platform": "macOS",
"platformVersion": "13.3.1"
}
  */


 public:
  bool GetUserAgent(std::string* out) {
    Val* val = nullptr;
    if (Find({"navigator", "userAgent"}, val) && val->is_string()) {
      *out = val->GetString();
      return true;
    }
    return false;
  }

  bool HasClientHints() {
    /**
     * Returns true if the highEntropyValues object is present
     * (navigator.userAgentData.getHighEntropyValues()).
     */
    return Find({"highEntropyValues"});
  }

  bool GetUserAgentClientHints(base::Value*& out) {
    /**
     * Returns the highEntropyValues object
     * (navigator.userAgentData.getHighEntropyValues()).
     */
    if (Find({"highEntropyValues"}, out) && out->is_dict()) {
      return true;
    }
    return false;
  }

  bool GetUserAgentMetadata(blink::UserAgentMetadata& out) {
    /**
     * Returns UACH from highEntropyValues object
     */
    base::Value* in = nullptr;
    if (GetUserAgentClientHints(in)) {
      return fingerprinting::utilities::user_agent::MakeUserAgentMetadata(
          in->GetDict(), out);
    }
    return false;
  }


  bool GetUserAgent(std::string& out) {
    /**
     * Returns user agent.
     */
    base::Value* in = nullptr;
    if (Find({"navigator", "userAgent"}, in) && in->is_string()) {
      out = in->GetString();
      return true;
    }
    return false;
  }

};

}  // namespace fingerprinting

#endif  // FINGERPRINTING_UA_MIXIN_H_
