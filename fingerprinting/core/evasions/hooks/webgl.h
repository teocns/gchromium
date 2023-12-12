#ifndef FINGERPRINTING_EVASIONS_HOOK_WEBBGL_H
#define FINGERPRINTING_EVASIONS_HOOK_WEBBGL_H

#include "base/component_export.h"
#include "fingerprinting/core/evasions/hook.h"

namespace fingerprinting::core::evasions {

class COMPONENT_EXPORT(FINGERPRINTING_CORE) hWebGL : public Hook {
 public:
  std::string codename() override { return "webgl"; }
  std::string get_impl(HookTargetType target) override {
    return R"(
        console.info("WebGL patch enabled for target. Argumets: ",
        arguments);
    )";
  }
};

// REGISTER_HOOK(webgl, hWebGL)

}  // namespace fingerprinting::core::evasions
#endif  // FINGERPRINTING_EVASIONS_HOOK_WEBBGL_H
