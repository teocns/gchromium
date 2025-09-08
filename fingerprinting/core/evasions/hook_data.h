
#ifndef FINGERPRINTING_CORE_EVASIONS_HOOK_DATA_H_
#define FINGERPRINTING_CORE_EVASIONS_HOOK_DATA_H_

#include <map>
#include "base/values.h"

namespace fingerprinting::core::evasions {
class HookData : public base::Value{
  /*
   * Reference to the portion of data that Hook needs to access from the Fingerprint object
   */


protected: 
  std::map<std::string, base::Value*> values_;
};


}

#endif  // FINGERPRINTING_CORE_EVASIONS_HOOK_DATA_H_
