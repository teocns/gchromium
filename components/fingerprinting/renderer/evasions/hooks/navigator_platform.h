#ifndef COMPONENTS_FINGERPRINTING_EVASIONS_HOOK_NAVIGATOR_PLATFORM_H
#define COMPONENTS_FINGERPRINTING_EVASIONS_HOOK_NAVIGATOR_PLATFORM_H

#include "base/component_export.h"
#include "components/fingerprinting/renderer/evasions/hook.h"
#include "components/fingerprinting/renderer/evasions/hook_factory.h"
namespace fingerprinting::core::evasions {

class hNavigatorPlatform : public Hook {
 public:
  std::string codename() override { return "navigator_platform"; }

  std::string get_impl() override {
    return R"(

let PatchAccessor = arguments[0].PatchAccessor
let dd = arguments[0].dd

function patchNavigatorPlatform(protoTarget){
  console.log("Patching...",protoTarget);
  PatchAccessor(
  protoTarget,
   // For workers its WorkerNavigator
  'platform',
  {
    get: function(target,thisArg,...args){
        Reflect.apply(target,thisArg,args);
        return dd.navigator.platform;
    }
  }
 )
}

if ('undefined' !== typeof globalThis.WorkerNavigator) {
    patchNavigatorPlatform(
        globalThis.WorkerNavigator.prototype,
    );
}
if ('undefined' !== typeof globalThis.Navigator) {
    patchNavigatorPlatform(
        globalThis.Navigator.prototype,
    );
}
    )";
  }
};

REGISTER_HOOK(navigator_platform, hNavigatorPlatform)

}  // namespace fingerprinting::core::evasions
#endif  // COMPONENTS_FINGERPRINTING_EVASIONS_HOOK_NAVIGATOR_PLATFORM_H
