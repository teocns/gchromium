#ifndef WEBGL_HOOK
#define WEBGL_HOOK

#include "fingerprinting/evasions/hook.h"
namespace fingerprinting {
namespace evasions {

class hWebGL final : public Hook {
  using Hook::Hook;
  std::string codename() override { return "webgl"; }
  std::string get_impl() override {
    return R"(
        console.info("WebGL patch enabled for target. Argumets: ", arguments);
    )";
  }
};

REGISTER_HOOK(webgl, hWebGL)


}  // namespace evasions
}  // namespace fingerprinting
#endif  // WEBGL_HOOK
