#ifndef COMPONENTS_FINGERPRINTING_EVASIONS_HOOK_WEBBGL_H
#define COMPONENTS_FINGERPRINTING_EVASIONS_HOOK_WEBBGL_H

#include "base/component_export.h"
#include "components/fingerprinting/renderer/evasions/hook_factory.h"
#include "components/fingerprinting/renderer/evasions/hook.h"

namespace fingerprinting::core::evasions {

class hWebGL : public Hook {
 public:
  static int priority() { return 1; }
  std::string codename() override { return "webgl"; }

  std::string get_impl() override {
    return R"(
    // Print the name of the arguments in a string format
    console.log(arguments)
    return true;
    )";
  }
};


// base::Value* get_data(Fingerprint* dd) {
//   base::Value* ptr = nullptr;
//   dd->GetWebGL(ptr);
//   return ptr;
// }


REGISTER_HOOK(webgl, hWebGL)

}  // namespace fingerprinting::core::evasions
#endif  // COMPONENTS_FINGERPRINTING_EVASIONS_HOOK_WEBBGL_H
