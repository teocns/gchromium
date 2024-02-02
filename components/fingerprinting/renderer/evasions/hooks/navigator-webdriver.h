
#ifndef COMPONENTS_FINGERPRINTING_EVASIONS_HOOK_NAVIGATOR_WEBDRIVER_H
#define COMPONENTS_FINGERPRINTING_EVASIONS_HOOK_NAVIGATOR_WEBDRIVER_H

#include "base/component_export.h"
#include "components/fingerprinting/renderer/evasions/hook.h"
#include "components/fingerprinting/renderer/evasions/hook_factory.h"
namespace fingerprinting::core::evasions {

class hNavigatorWebdriver : public Hook {
 public:
  using Hook::Hook;
  std::string codename() override { return "navigator_webdriver"; }

  std::string get_impl() override {
    return R"(


const utils = arguments[0].utils;

        debugger;

const webdriverDesc =
    utils.cache.global.Navigator.prototype.webdriver ||
    utils.cache.global.WorkerNavigator.prototype.webdriver;

if (webdriverDesc === undefined) {
    // Post Chrome 89.0.4339.0 and already good
    return;
}

// invoke the original getter of prototype, *DO NOT* use the code like: ' navigator.webdriver === false '
const get_webdriverFunc = webdriverDesc.get.bind(
    utils.cache.window.navigator
);

if (get_webdriverFunc() === false) {
    // Pre Chrome 89.0.4339.0 and already good
    return;
}

// Pre Chrome 88.0.4291.0 and needs patching
delete Object.getPrototypeOf(navigator).webdriver;
    )";
  }
};

REGISTER_HOOK(navigator_webdriver, hNavigatorWebdriver, 0)

}  // namespace fingerprinting::core::evasions
#endif  // COMPONENTS_FINGERPRINTING_EVASIONS_HOOK_NAVIGATOR_WEBDRIVER_H
