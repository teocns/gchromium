#ifndef FINGERPRINTING_EVASIONS_HOOK_WEBBGL_H
#define FINGERPRINTING_EVASIONS_HOOK_WEBBGL_H

#include "base/component_export.h"
#include "fingerprinting/core/evasions//hook_factory.h"
#include "fingerprinting/core/evasions/hook.h"

namespace fingerprinting::core::evasions {

class COMPONENT_EXPORT(FINGERPRINTING_CORE_EVASIONS) hWebGL : public Hook {
 public:
  std::string codename() override { return "webgl"; }

  std::string get_impl() override {
    return R"(
      try{
console.log([PatchValue,PatchAccessor])



globalThis.PatchAccessor = PatchAccessor;
globalThis.PatchValue = PatchValue;

  }catch(e){
    console.error(e)
  }
    )";
  }
};

REGISTER_HOOK(webgl, hWebGL)

}  // namespace fingerprinting::core::evasions
#endif  // FINGERPRINTING_EVASIONS_HOOK_WEBBGL_H
