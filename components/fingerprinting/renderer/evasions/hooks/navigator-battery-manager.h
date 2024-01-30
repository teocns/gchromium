
#ifndef COMPONENTS_FINGERPRINTING_EVASIONS_HOOK_NAVIGATOR_BATTERY_MANAGER_H
#define COMPONENTS_FINGERPRINTING_EVASIONS_HOOK_NAVIGATOR_BATTERY_MANAGER_H

#include "base/component_export.h"
#include "components/fingerprinting/renderer/evasions/hook.h"
#include "components/fingerprinting/renderer/evasions/hook_factory.h"
namespace fingerprinting::core::evasions {

class hNavigatorBatteryManager : public Hook {
 public:
  static int priority() { return 0; }
  std::string codename() override { return "navigator_battery_manager"; }

  std::string get_impl() override {
    return R"(
const utils = arguments[0].utils;

if ('undefined' != typeof BatteryManager) {
    utils.replaceGetterWithProxy(
        BatteryManager.prototype,
        'charging',
        function(target, thisArg, ...args) {
            Reflect.apply(target,thisArg, args);
            return true;
        }
    );

    utils.replaceGetterWithProxy(
        BatteryManager.prototype,
        'chargingTime',
        function(target, thisArg, ...args) {
            Reflect.apply(target,thisArg, args);
            return Infinity;
        }
    );

    utils.replaceGetterWithProxy(
        BatteryManager.prototype,
        'dischargingTime',
        function(target, thisArg, ...args) {
            Reflect.apply(target,thisArg, args);
            return Infinity;
        }
    );

    utils.replaceGetterWithProxy(
        BatteryManager.prototype,
        'level',
        function(target, thisArg, ...args) {
            Reflect.apply(target,thisArg, args);
            return 1;
        }
    );
}
    )";
  }
};

REGISTER_HOOK(navigator_battery_manager, hNavigatorBatteryManager)

}  // namespace fingerprinting::core::evasions
#endif  // COMPONENTS_FINGERPRINTING_EVASIONS_HOOK_NAVIGATOR_BATTERY_MANAGER_H
