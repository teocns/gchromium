const PatchAccessor = arguments[0].PatchAccessor;
const PatchValue = arguments[0].PatchValue;
function magic() {
  utils = {
    cache: {
      global: 'undefined' !== typeof window ? window : globalThis,
      window: {
        getComputedStyle:
          'undefined' !== typeof window && window.getComputedStyle.bind(window),
        eval:
          'undefined' !== typeof window
            ? window.eval.bind(window)
            : globalThis
              ? globalThis.eval.bind(globalThis)
              : undefined,
        navigator:
          'undefined' !== typeof window
            ? window.navigator
            : globalThis
              ? globalThis.navigator
              : undefined,
      },
      OffscreenCanvas: {
        prototype: {
          getContext:
            'undefined' !== typeof OffscreenCanvas &&
            OffscreenCanvas.prototype.getContext,
        },
      },
      HTMLCanvasElement: {
        prototype: {
          getContext:
            'undefined' !== typeof HTMLCanvasElement &&
            HTMLCanvasElement.prototype.getContext,
        },
      },
      Descriptor: {},
    },
  };

  utils.variables = {
    renderingContextWithOperators: [],
    taskData: {},
    // constant values
    kObjPlaceHolder: '_$obj!_//+_',
    kObjUndefinedPlaceHolder: '_$obj!_undefined_//+_',
  };

  utils.ogProxyAdapter = function(src) {
    // HACK: We're cheaping out on refactoring so old evasions versions still pass an { apply: fun } object as the third arguments
    // We check for this and if it's the case, we just use the function directly

    if (typeof src === 'function') {
      return src;
    }
    else if (typeof src === 'object'){
      throw new Error('Invalid ogProxyAdapter');
    }

    if (src.hasOwnProperty('apply')) {
      return src.apply;
    }
    else if (src.hasOwnProperty('constructor')) {
      // We need to override the constructor here
      // Basically we create a new function that returns th
    }

    
  };

  utils.replaceGetterWithProxy = function(obj, name, getter) {
    PatchAccessor(obj, name, {
      get: utils.ogProxyAdapter(getter),
    });
  };
  utils.replaceSetterWithProxy = function(obj, name, setter) {
    PatchAccessor(obj, name, {
      set: utils.ogProxyAdapter(setter),
    });
  };

  utils.replaceWithProxy = function(obj, name, fun) {
    PatchValue(obj, name, utils.ogProxyAdapter(fun));
  };
  /**
   * The context is saved when the canvas.getContext is created.
   * @param context
   * @param operatorName
   * @returns {number}
   */
  utils.markRenderingContextOperator = (context, operatorName) => {
    const result = utils.variables.renderingContextWithOperators.findIndex(
      (e) => e.context === context,
    );

    if (result >= 0) {
      const operators = utils.variables.renderingContextWithOperators[result];
      if (operators) {
        operators.operators[operatorName] = true;
      }
    }

    return result;
  };

  /**
   * Find the context created by the external based on the canvas
   * @param canvas
   * @returns {{context: *, contextIndex: number}|{context: null, contextIndex: number}}
   */
  utils.findRenderingContextIndex = (canvas) => {
    const contextIds = [
      '2d',
      'webgl',
      'experimental-webgl',
      'webgl2',
      'experimental-webgl2',
      'bitmaprenderer',
    ];

    for (const contextId of contextIds) {
      let context = null;

      if (
        utils.cache.Object.getPrototypeOf(canvas) === OffscreenCanvas.prototype
      ) {
        context = utils.cache.OffscreenCanvas.prototype.getContext.call(
          canvas,
          contextId,
        );
      } else {
        context = utils.cache.HTMLCanvasElement.prototype.getContext.call(
          canvas,
          contextId,
        );
      }

      const contextIndex =
        utils.variables.renderingContextWithOperators.findIndex(
          (e) => e.context === context,
        );

      if (contextIndex >= 0) {
        return { context, contextIndex };
      }
    }

    return { context: null, contextIndex: -1 };
  };

  utils.overwriteObjectProperties = function(obj, newPropValues, blackList) {
    if (!obj) {
      return;
    }

    for (const name in newPropValues) {
      if (blackList && blackList.includes(name)) {
        continue;
      }

      // Check if the original has this property
      const desc = Object.getOwnPropertyDescriptor(obj, name);
      if (!desc) {
        continue;
      }

      let replaceFn = null;

      if ('undefined' !== typeof desc['value']) {
        replaceFn = utils.replaceWithProxy;
      } else if ('undefined' !== typeof desc['get']) {
        replaceFn = utils.replaceGetterWithProxy;
      } else {
        if ('value' in desc) {
          replaceFn = utils.replaceWithProxy;
        } else if ('get' in desc) {
          replaceFn = utils.replaceGetterWithProxy;
        } else {
          replaceFn = utils.replaceSetterWithProxy;
        }
      }

      let newPropValue = newPropValues[name];

      // maybe it's a function...
      if ('function' === typeof newPropValue) {
        newPropValue = newPropValue();
      }

      if (newPropValue === utils.variables.kObjPlaceHolder) {
        // If it contains attribute and has value, exit directly
        continue;
      }

      // PatchValue(
      //   Function.prototype,
      //   'toString',
      //   function(){
      //     return "hello";
      //   }
      // )

      replaceFn(obj, name, {
        apply(target, thisArg, ...args) {
          const result = Reflect.apply(target, thisArg, args);

          let newPropValue = newPropValues[name];

          // maybe it's a function...
          if ('function' === typeof newPropValue) {
            newPropValue = newPropValue(thisArg);
          }

          if (newPropValue === utils.variables.kObjUndefinedPlaceHolder) {
            newPropValue = undefined;
          }

          if ('undefined' == typeof newPropValue) {
            // If empty, delete this property
            delete obj[name];
          } else {
            // Other value, direct assignment
          }

          // Consider whether the check for undefined elsewhere contains
          if ('undefined' == typeof newPropValue) {
            // TODO: Ignore this property
            // utils.addIgnoreProperty(o, propertyKey);
          }

          return Object.freeze(newPropValue);
        },
      });
    }
  };

  utils.makeGetter = (value) => ({
    apply(target, thisArg, ...args) {
      // Let's fetch the value first, to trigger and escalate potential errors
      // Illegal invocations like `navigator.__proto__.vendor` will throw here
      Reflect.apply(...arguments);
      return value;
    },
  });

  return utils;
}

// arguments[0] is the object shared between all hooks
arguments[0].utils = magic();
globalThis.patchAccessor = arguments[0].PatchAccessor;
globalThis.patchValue = arguments[0].PatchValue;
