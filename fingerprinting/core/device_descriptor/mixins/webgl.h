#ifndef FINGERPRINTING_WEBGL_MIXIN_H
#define FINGERPRINTING_WEBGL_MIXIN_H

#include "base/values.h"
#include "fingerprinting/core/device_descriptor/fingerprint.h"
#include "fingerprinting/utility/user_agent.h"


namespace fingerprinting {

class COMPONENT_EXPORT(FINGERPRINTING_CORE_EVASIONS_DEVICE_DESCRIPTOR)
    WebGLMixin : protected virtual IFingerprint {
 public:
  bool GetWebGL(base::Value*& out) {
    /**
     * Returns the vendor of the primary WebGL context.
     * (gl.getParameter(gl.VENDOR))
     */
    return Find({"webgl"}, out);
  }
};
}  // namespace fingerprinting

#endif  // FINGERPRINTING_WEBGL_MIXIN_H
