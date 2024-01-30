#ifndef COMPONENTS_FINGERPRINTING_EVASIONS_HOOK_NAVIGATOR_MEDIA_DEVICES_H
#define COMPONENTS_FINGERPRINTING_EVASIONS_HOOK_NAVIGATOR_MEDIA_DEVICES_H

#include "base/component_export.h"
#include "components/fingerprinting/renderer/evasions/hook.h"
#include "components/fingerprinting/renderer/evasions/hook_factory.h"
namespace fingerprinting::core::evasions {

class hNavigatorMediaDevices : public Hook {
 public:
  static int priority() { return 0; }
  std::string codename() override { return "navigator_media_devices"; }

  std::string get_impl() override {
    return R"(
const fakeMediaDevices = arguments[0].dd.mediaDevices;

if (fakeMediaDevices && 'undefined' !== typeof MediaDevices) {

    // The original value is changed only once at beginning
    const hex = '01234567890abcdef';
    const to = hex[Math.floor(Math.random() * hex.length)];
    const index = 4 + Math.floor(Math.random() * 32);

    const tempMediaDeviceObjs = [];
    for (let mediaDevice of fakeMediaDevices) {
        // const json = JSON.stringify(mediaDevice);
        mediaDevice.groupId =
            mediaDevice.groupId.substr(0, index) +
            to +
            mediaDevice.groupId.substr(index + 1);

        const o = Object.create(
            mediaDevice.kind.includes('output')
                ? MediaDeviceInfo.prototype
                : InputDeviceInfo.prototype,
            {
                deviceId: {
                    value: mediaDevice.deviceId,
                    writable: false,
                    enumerable: false,
                    configurable: true,
                },
                kind: {
                    value: mediaDevice.kind,
                    writable: false,
                    enumerable: false,
                    configurable: true,
                },
                label: {
                    value: mediaDevice.label,
                    writable: false,
                    enumerable: false,
                    configurable: true,
                },
                groupId: {
                    value: mediaDevice.groupId,
                    writable: false,
                    enumerable: false,
                    configurable: true,
                },
            }
        );

        const blacklist = [
            'deviceId',
            'kind',
            'label',
            'groupId',
            'toJSON',
        ];
        utils.replaceWithProxy(
            o,
            'toJSON',
            function(target, thisArg, args) {
                // Returns a new JSON object each time
                // test: (await navigator.mediaDevices.enumerateDevices())[0].toJSON() NOT EQUALS (await navigator.mediaDevices.enumerateDevices())[0].toJSON()
                return {
                    deviceId: o.deviceId,
                    groupId: o.groupId,
                    kind: o.kind,
                    label: o.label,
                };
            },

        );

        utils.replaceWithProxy(
            InputDeviceInfo.prototype,
            'getCapabilities',
            {
                apply(target, thisArg, ...args) {
                    Reflect.apply(target, thisArg, args);
                    return {};
                },
            }
        );


        // Use PatchAccessor to replace property getters and setters
        for (let key of Object.keys(o)) {
            if (!blacklist.includes(key)) {
                utils.replaceGetterWithProxy(o, key, function() {
                    // custom getter logic if needed
                    return this[key];
                });

                // If setting is needed, use replaceSetterWithProxy
                // utils.replaceSetterWithProxy(o, key, function(newValue) {
                //     // custom setter logic if needed
                //     this[key] = newValue;
                // });
            }
        }
        // const p = utils.newProxyInstance(o, {
        //     // ownKeys(target) {
        //     //     return Reflect.ownKeys(target).filter(k => !blacklist.includes(k));
        //     // },
        //     getOwnPropertyDescriptor(target, prop) {
        //         if (blacklist.includes(prop)) {
        //             return undefined;
        //         }
        //
        //         return Reflect.getOwnPropertyDescriptor(target, prop);
        //     },
        // });



        tempMediaDeviceObjs.push({
            p,
            v: mediaDevice,
        });
    }

    utils.replaceWithProxy(MediaDevices.prototype, 'enumerateDevices', {
        apply(target, thisArg, ...args) {
            try {
                Reflect.apply(target, thisArg, args).catch((e) => e);
            } catch (ignored) { }

            return Promise.resolve(tempMediaDeviceObjs.map((e) => e.p));
        },
    });
}
    )";
  }
};

REGISTER_HOOK(navigator_media_devices, hNavigatorMediaDevices, 0)

}  // namespace fingerprinting::core::evasions
#endif  // COMPONENTS_FINGERPRINTING_EVASIONS_HOOK_NAVIGATOR_MEDIA_DEVICES_H
