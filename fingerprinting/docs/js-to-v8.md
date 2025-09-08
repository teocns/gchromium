## Migrating JavaScript evasions to V8


We are going to port JavaScript evasions extracted from puppeteer-stealth-extra plugin into the V8 engine of Chromium.

The JavaScript framework leverages on utility functions that mock objects with a `Proxy` (see `utils.replaceWithProxy`) to tamper their handles. The Proxy object enables you to create a proxy for another object, which can intercept and redefine fundamental operations for that object.

Below is the definition of the aforementioned utilities:

```
utils.replaceProperty = (obj, propName, descriptorOverrides = {}) => {
    const _Object = utils.cache.Object;
    const descriptor = _Object.getOwnPropertyDescriptor(obj, propName) || {};

    if (descriptor) {
        return _Object.defineProperty(obj, propName, {
            // Copy over the existing descriptors (writable, enumerable, configurable, etc)
            ...descriptor,
            // Add our overrides (e.g. value, get())
            ...descriptorOverrides,
        });
    }
};


utils.replaceWithProxy = (obj, propName, handler) => {
    const originalObj = obj[propName];
    const _Reflect = utils.cache.Reflect;

    if (!originalObj) {
        return false;
    }

    const proxyObj = utils.newProxyInstance(
        originalObj,
        utils.stripProxyFromErrors(handler)
    );

    utils.replaceProperty(obj, propName, { value: proxyObj });

    return true;
};


utils.newProxyInstance = (target, handler) => {
    // const newTarget = utils.getProxyTarget(target);
    const result = new Proxy(target, handler);

    utils.replaceProperty(result, Symbol.hasInstance, {
        value: function (instance) {
            let err;
            try {
                Object.setPrototypeOf(instance, instance) + '';
            } catch (e) {
                if (e.name === 'RangeError')
                    err = utils.patchError(
                        new TypeError(
                            "Function has non-object prototype 'undefined' in instanceof check"
                        ),
                        'Proxy'
                    );
                else
                    err = utils.patchError(
                        new TypeError(
                            "Function has non-object prototype 'undefined' in instanceof check"
                        ),
                        'Function'
                    );
            } finally {
                throw err;
            }
        },
        configurable: true,
        enumerable: false,
        writable: false,
    });

    utils.variables.proxies.push({ proxy: result, target });
    return result;
};

```



The problem with this tampering approach is that there are a variety of anti-tampering frameworks, like CreepJS, that can detect objects' tampering via the usage of `Proxy`.


Below is an example of how an User-Agent evasion is implemented:

```

hookFunction = (utils, evasionOpts, hookData) => {
        const { uaOverride, highEntropyValues } = hookData;

        if ('undefined' !== typeof NavigatorUAData) {
            const _Reflect = utils.cache.Reflect;

            const emptyBrands = [
                {
                    brand: '',
                    version: '',
                },
            ];

            // brands
            utils.replaceGetterWithProxy(NavigatorUAData.prototype, 'brands', {
                apply(target, thisArg, args) {
                    _Reflect.apply(target, thisArg, args);

                    // when window removed from parent, navigator's values changed to empty:
                    if (
                        'undefined' !== typeof Window &&
                        globalThis instanceof Window &&
                        !globalThis.parent
                    ) {
                        return JSON.parse(JSON.stringify(emptyBrands));
                    }

                    return JSON.parse(JSON.stringify(highEntropyValues.brands));
                },
            });

            // mobile
            utils.replaceGetterWithProxy(NavigatorUAData.prototype, 'mobile', {
                apply(target, thisArg, args) {
                    _Reflect.apply(target, thisArg, args);

                    // when window removed from parent, navigator's values changed to empty:
                    if (
                        'undefined' !== typeof Window &&
                        globalThis instanceof Window &&
                        !globalThis.parent
                    ) {
                        return false;
                    }

                    return highEntropyValues.mobile;
                },
            });

            // platform
            utils.replaceGetterWithProxy(
                NavigatorUAData.prototype,
                'platform',
                {
                    apply(target, thisArg, args) {
                        _Reflect.apply(target, thisArg, args);

                        // when window removed from parent, navigator's values changed to empty:
                        if (
                            'undefined' !== typeof Window &&
                            globalThis instanceof Window &&
                            !globalThis.parent
                        ) {
                            return '';
                        }

                        return highEntropyValues.platform;
                    },
                }
            );

            // getHighEntropyValues
            utils.replaceWithProxy(
                NavigatorUAData.prototype,
                'getHighEntropyValues',
                {
                    apply(target, thisArg, args) {
                        return new utils.cache.Promise((resolve, reject) => {
                            // when window removed from parent, this method will return `undefined`
                            const origApplyResult = _Reflect.apply(
                                target,
                                thisArg,
                                args
                            );

                            if (
                                !origApplyResult ||
                                !(origApplyResult instanceof Promise)
                            ) {
                                return resolve(origApplyResult);
                            }

                            origApplyResult
                                .then((origResult) => {
                                    if (!args.length) {
                                        return resolve(origResult);
                                    }

                                    const hints = Array.from(...args);

                                    // after test, if args is an empty Array, return empty Object
                                    if (!hints.length) {
                                        return resolve(origResult);
                                    }

                                    // otherwise, at least brands and mobile
                                    const result = {
                                        brands: highEntropyValues.brands,
                                        mobile: highEntropyValues.mobile,
                                    };

                                    for (const n of hints) {
                                        if (n in highEntropyValues) {
                                            result[n] = highEntropyValues[n];
                                        }
                                    }

                                    resolve(JSON.parse(JSON.stringify(result)));
                                })
                                .catch((ex) => {
                                    reject(utils.patchError(ex, 'apply'));
                                });
                        });
                    },
                }
            );

            // noinspection JSUnresolvedVariable
            utils.replaceWithProxy(NavigatorUAData.prototype, 'toJSON', {
                apply(target, thisArg, args) {
                    _Reflect.apply(target, thisArg, args);

                    let result;

                    // when window removed from parent, navigator's values changed to empty:
                    if (
                        'undefined' !== typeof Window &&
                        globalThis instanceof Window &&
                        !globalThis.parent
                    ) {
                        result = {
                            brands: emptyBrands,
                            mobile: false,
                        };
                    } else {
                        result = {
                            brands: highEntropyValues.brands,
                            mobile: highEntropyValues.mobile,
                        };
                    }

                    return JSON.parse(JSON.stringify(result));
                },
            });
        }
    };

```



In order to avoid having to use `Proxy` as a tampering gateway, let's take the following approach:
Provide the implementation for an equivalent of `utils.replaceWithProxy` handler in C++, name it `replaceHandler`. 
This function will be eventually injected in the javascript globals and mimicks the same signature as `utils.replaceWithProxy`, however, instead of creating a Proxy to override handlers, we override the handler by directly patching V8 objects.
Remember that when invoked from javascript, the parameters are:

1. Object target 
2. String propName
3. Object (dictionary) where we specify handler overrides, i.e {get: ()=> {...}, set: ()=>{...}}

