#ifndef COMPONENTS_FINGERPRINTING_EVASIONS_HOOK_NAVIGATOR_H
#define COMPONENTS_FINGERPRINTING_EVASIONS_HOOK_NAVIGATOR_H

#include "base/component_export.h"
#include "components/fingerprinting/renderer/evasions/hook.h"
#include "components/fingerprinting/renderer/evasions/hook_factory.h"
namespace fingerprinting::core::evasions {

class hNavigator: public Hook {
 public:
  static int priority() { return 0; }
  std::string codename() override { return "navigator"; }

  std::string get_impl() override {
    return R"(
const fakeNavigator = arguments[0].dd.navigator;
const t = {};
const utils = arguments[0].utils;
const g_window = utils.cache.global;
if ('undefined' !== typeof g_window.WorkerNavigator) {
  utils.overwriteObjectProperties(
    g_window.WorkerNavigator.prototype,
    fakeNavigator,
  );
}
if ('undefined' !== typeof g_window.Navigator) {
  for (const k in fakeNavigator) {
    const v = fakeNavigator[k];
    t[k] = (navigator) => {
      if (!navigator) {
        return v;
      }

      if (!globalThis.parent) {
        if (k === 'appVersion') {
          return '';
        } else if (k === 'userAgent') {
          return '';
        }
      }
      return v;
    };

    utils.overwriteObjectProperties(g_window.Navigator.prototype, t, [
      'batteryManager',
      'connection',
      'webdriver',
      'pdfViewerEnabled',
      'userAgent',
      'appVersion',
    ]);
  }
}
    )";
  }
};

REGISTER_HOOK(navigator, hNavigator)

}  // namespace fingerprinting::core::evasions
#endif  // COMPONENTS_FINGERPRINTING_EVASIONS_HOOK_NAVIGATOR_H
