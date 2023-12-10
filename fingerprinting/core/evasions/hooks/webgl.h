#ifndef WEBGL_HOOK_IMPL
#define WEBGL_HOOK_IMPL

#include "fingerprinting/core/evasions/hook.h"

namespace fingerprinting::evasions {

class hWebGL : public Hook {
  std::string codename() override { return "webgl"; }
  std::string get_impl(HookTargetType target) override {
    return R"(
        console.info("WebGL patch enabled for target. Argumets: ", arguments);
    )";
  }
};

REGISTER_HOOK(webgl, hWebGL)


}  // namespace fingerprinting
#endif  // WEBGL_HOOK_IMPL
