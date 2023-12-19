
#include "fingerprinting/core/evasions/hook.h"
#include "fingerprinting/core/evasions/pack.h"
#include "v8/include/v8-context.h"
#include "v8/include/v8-local-handle.h"

namespace fingerprinting::evasions {

namespace {
class Package;
}

class Hook : public ::fingerprinting::core::evasions::Hook {
  // explicit Hook(::fingerprinting::core::evasions::Hook hook_,
  // std::shared_ptr<Package> package_) : hook(hook_), package(package_) {}
 public:
  using ::fingerprinting::core::evasions::Hook::Hook;

  std::shared_ptr<Package> package;

  v8::MaybeLocal<v8::Function> GetFunction(v8::Local<v8::Context> context);
};

}  // namespace fingerprinting::evasions
