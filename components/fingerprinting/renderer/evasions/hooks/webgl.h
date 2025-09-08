#ifndef COMPONENTS_FINGERPRINTING_EVASIONS_HOOK_WEBBGL_H
#define COMPONENTS_FINGERPRINTING_EVASIONS_HOOK_WEBBGL_H

#include "base/component_export.h"
#include "components/fingerprinting/renderer/evasions/hook.h"
#include "components/fingerprinting/renderer/evasions/hook_factory.h"

namespace fingerprinting::core::evasions {

class hWebGL : public Hook {
 public:
  using Hook::Hook;
  std::string codename() override { return "webgl"; }

  std::string get_impl() override {
    return R"(
const utils = arguments[0].utils;
const g_window = utils.cache.global;

const fakeWebGL = {
  gpu: arguments[0].dd.gpu,
  webgl: arguments[0].dd.webgl,
  webgl2: arguments[0].dd.webgl2,
};

const shaderPrecisionFormats = [];

const WebGLShaderPrecisionFormat_prototype_rangeMin_get =
  Object.getOwnPropertyDescriptor(WebGLShaderPrecisionFormat.prototype, 'rangeMin').get;
const WebGLShaderPrecisionFormat_prototype_rangeMax_get =
  Object.getOwnPropertyDescriptor(WebGLShaderPrecisionFormat.prototype, 'rangeMax').get;
const WebGLShaderPrecisionFormat_prototype_precision_get =
  Object.getOwnPropertyDescriptor(WebGLShaderPrecisionFormat.prototype, 'precision').get;

const bindContext = (__WebGLRenderingContext, fakeDDPropName) => {
  // getParameter
  utils.replaceWithProxy(
    __WebGLRenderingContext.prototype,
    'getParameter',
    {
      apply(target, thisArg, ...args) {
        // We must call this primitive method, and akamai will listen to see if this primitive method is called
        const origResult = Reflect.apply(
          target,
          thisArg,
          args
        );
        const type = args[0];
        let result = undefined;

        switch (type) {
          case 37445 /* renderer.UNMASKED_VENDOR_WEBGL */:
            result = fakeWebGL.gpu.vendor;
            break;

          case 37446 /* renderer.UNMASKED_RENDERER_WEBGL */:
            result = fakeWebGL.gpu.renderer;
            break;

          default:
            const param =
              fakeWebGL[fakeDDPropName].params[type];
            if (param) {
              const paramValue = param.value;

              if (
                paramValue &&
                paramValue.constructor.name === 'Object'
              ) {
                const classType = param.type;
                // Float32Array, Int32Array, ...
                result = new utils.cache.global[
                  classType
                ](Object.values(paramValue));
              } else if (
                paramValue &&
                paramValue.constructor.name === 'Array'
              ) {
                // array, need to return new Array
                // !!! context.getParameter(3107) MUST NOT EQUALS context.getParameter(3107)
                result = [...paramValue];
              } else {
                // including: null, number, string, array
                result = paramValue;
              }
            }

            break;
        }

        if (result === undefined) {
          result = origResult;
        }

        return result;
      },
    }
  );

  // noinspection JSUnusedLocalSymbols
  utils.replaceWithProxy(
    WebGLRenderingContext.prototype,
    'getSupportedExtensions',
    {
      apply(target, thisArg, ...args) {
        Reflect.apply(target, thisArg, args);
        return [
          ...fakeWebGL[fakeDDPropName].supportedExtensions,
        ];
      },
    }
  );

  // getContextAttributes
  utils.replaceWithProxy(
    WebGLRenderingContext.prototype,
    'getContextAttributes',
    {
      apply(target, thisArg, ...args) {
        const result = Reflect.apply(target, thisArg, args);

        result.alpha =
          fakeWebGL[fakeDDPropName].contextAttributes.alpha;
        result.antialias =
          fakeWebGL[
            fakeDDPropName
          ].contextAttributes.antialias;
        result.depth =
          fakeWebGL[fakeDDPropName].contextAttributes.depth;
        result.desynchronized =
          fakeWebGL[
            fakeDDPropName
          ].contextAttributes.desynchronized;
        result.failIfMajorPerformanceCaveat =
          fakeWebGL[
            fakeDDPropName
          ].contextAttributes.failIfMajorPerformanceCaveat;
        result.powerPreference =
          fakeWebGL[
            fakeDDPropName
          ].contextAttributes.powerPreference;
        result.premultipliedAlpha =
          fakeWebGL[
            fakeDDPropName
          ].contextAttributes.premultipliedAlpha;
        result.preserveDrawingBuffer =
          fakeWebGL[
            fakeDDPropName
          ].contextAttributes.preserveDrawingBuffer;
        result.stencil =
          fakeWebGL[fakeDDPropName].contextAttributes.stencil;
        result.xrCompatible =
          fakeWebGL[
            fakeDDPropName
          ].contextAttributes.xrCompatible;

        return result;
      },
    }
  );

  // getShaderPrecisionFormat
  utils.replaceWithProxy(
    WebGLRenderingContext.prototype,
    'getShaderPrecisionFormat',
    {
      apply(target, thisArg, ...args) {
        const shaderPrecisionFormat = Reflect.apply(
          target,
          thisArg,
          args
        );

        shaderPrecisionFormats.push({
          shaderPrecisionFormat,
          webglPropName: fakeDDPropName,
          shaderType: args[0],
          precisionType: args[1],
          rangeMin:
            WebGLShaderPrecisionFormat_prototype_rangeMin_get.call(
              shaderPrecisionFormat
            ),
          rangeMax:
            WebGLShaderPrecisionFormat_prototype_rangeMax_get.call(
              shaderPrecisionFormat
            ),
          precision:
            WebGLShaderPrecisionFormat_prototype_precision_get.call(
              shaderPrecisionFormat
            ),
        });

        return shaderPrecisionFormat;
      },
    }
  );
};

// WebGLRenderingContext.STENCIL_BACK_PASS_DEPTH_FAIL;
bindContext(WebGLRenderingContext, 'webgl');
bindContext(WebGL2RenderingContext, 'webgl2');

// WebGLShaderPrecisionFormat
// noinspection JSUnusedLocalSymbols
utils.replaceGetterWithProxy(
  WebGLShaderPrecisionFormat.prototype,
  'precision',
  {
    apply(target, thisArg, ...args) {
      Reflect.apply(target, thisArg, args);

      const r = shaderPrecisionFormats.find(
        (e) => e.shaderPrecisionFormat === thisArg
      );

      // webglPropName
      // shaderType
      // precisionType,
      // rangeMin
      // rangeMax
      // precision
      const {
        webglPropName,
        shaderType,
        precisionType,
        rangeMin,
        rangeMax,
        precision,
      } = r;

      const fake_r = fakeWebGL[
        webglPropName
      ].shaderPrecisionFormats.find(
        (e) =>
          e.shaderType === shaderType &&
          e.precisionType === precisionType
      );

      const result = fake_r ? fake_r.r.precision : precisionType;
      return result;
    },
  }
);

// noinspection JSUnusedLocalSymbols
utils.replaceGetterWithProxy(
  WebGLShaderPrecisionFormat.prototype,
  'rangeMin',
  {
    apply(target, thisArg, ...args) {
      Reflect.apply(target, thisArg, args);

      const r = shaderPrecisionFormats.find(
        (e) => e.shaderPrecisionFormat === thisArg
      );

      const {
        webglPropName,
        shaderType,
        precisionType,
        rangeMin,
        rangeMax,
        precision,
      } = r;

      const fake_r = fakeWebGL[
        webglPropName
      ].shaderPrecisionFormats.find(
        (e) =>
          e.shaderType === shaderType &&
          e.precisionType === precisionType
      );

      const result = fake_r ? fake_r.r.rangeMin : rangeMin;
      return result;
    },
  }
);

// noinspection JSUnusedLocalSymbols
utils.replaceGetterWithProxy(
  WebGLShaderPrecisionFormat.prototype,
  'rangeMax',
  {
    apply(target, thisArg, ...args) {
      Reflect.apply(target, thisArg, args);

      const r = shaderPrecisionFormats.find(
        (e) => e.shaderPrecisionFormat === thisArg
      );

      const {
        webglPropName,
        shaderType,
        precisionType,
        rangeMin,
        rangeMax,
        precision,
      } = r;

      const fake_r = fakeWebGL[
        webglPropName
      ].shaderPrecisionFormats.find(
        (e) =>
          e.shaderType === shaderType &&
          e.precisionType === precisionType
      );

      const result = fake_r ? fake_r.r.rangeMax : rangeMax;
      return result;
    },
  }
)
    )";
  }
};

// base::Value* get_data(Fingerprint* dd) {
//   base::Value* ptr = nullptr;
//   dd->GetWebGL(ptr);
//   return ptr;
// }

REGISTER_HOOK(webgl, hWebGL, 1)

}  // namespace fingerprinting::core::evasions
#endif  // COMPONENTS_FINGERPRINTING_EVASIONS_HOOK_WEBBGL_H
