#ifndef FINGERPRINTING_EVASIONS_HOOK_WEBBGL_H
#define FINGERPRINTING_EVASIONS_HOOK_WEBBGL_H

#include "base/component_export.h"
#include "fingerprinting/core/evasions/hook_factory.h"
#include "fingerprinting/core/evasions/hook.h"

namespace fingerprinting::evasions {

class COMPONENT_EXPORT(FINGERPRINTING_CORE_EVASIONS) hWebGL : public Hook {
 public:
  std::string codename() override { return "webgl"; }

  std::string get_impl() override {
    return R"(
    // Print the name of the arguments in a string format
    console.log(arguments.dd)
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

}  // namespace fingerprinting::evasions
#endif  // FINGERPRINTING_EVASIONS_HOOK_WEBBGL_H
