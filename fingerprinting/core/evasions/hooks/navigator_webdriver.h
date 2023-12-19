#ifndef FINGERPRINTING_EVASIONS_HOOK_WEBBGL_H
#define FINGERPRINTING_EVASIONS_HOOK_WEBBGL_H

#include "base/component_export.h"
#include "fingerprinting/core/evasions/hook.h"

namespace fingerprinting::core::evasions {

class COMPONENT_EXPORT(FINGERPRINTING_CORE_EVASIONS) hNavigatorWebdriver : public Hook {
 public:
  std::string codename = "navigator_webdriver";
  std::string impl = R"(
PatchAccessor(
 Navigator.prototype,
 'webdriver',
 {
   get: function(target,thisArg,...args){
     return false;
   }
 }
)
    )";
};

REGISTER_HOOK(navigator_webdriver, hNavigatorWebdriver)

}  // namespace fingerprinting::core::evasions
#endif  // FINGERPRINTING_EVASIONS_HOOK_WEBBGL_H
