#ifndef COMPONENTS_FINGERPRINTING_EVASIONS_HOOK_NAVIGATOR_WEBDRIVER_H
#define COMPONENTS_FINGERPRINTING_EVASIONS_HOOK_NAVIGATOR_WEBDRIVER_H

#include "base/component_export.h"
#include "fingerprinting/core/evasions/hook.h"
#include "fingerprinting/core/evasions/hook_factory.h"
namespace fingerprinting::core::evasions {

class hNavigatorWebdriver : public Hook {
 public:
  std::string codename() override { return "navigator_webdriver"; }

  std::string get_impl() override {
    return R"(
     // Print the name of the arguments in a string format
     arguments.PatchAccessor(
      Navigator.prototype,
      'webdriver',
      {
        get: function(target,thisArg,...args){
          return false;
        }
      }
     )
    )";
  }
};

REGISTER_HOOK(navigator_webdriver, hNavigatorWebdriver)

}  // namespace fingerprinting::core::evasions
#endif  // COMPONENTS_FINGERPRINTING_EVASIONS_HOOK_NAVIGATOR_WEBDRIVER_H
