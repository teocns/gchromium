#ifndef FINGERPRINTING_EVASIONS_HOOK_H_
#define FINGERPRINTING_EVASIONS_HOOK_H_

#include <format>
#include <random>
#include <string>
#include <map>
#include "v8/include/v8.h"

namespace fingerprinting {
namespace evasions {


enum HookTargetType {
  PAGE,
  WORKER,
  SHARED_WORKER,
};

struct HookTarget {
  // Where the hook will inject / bound
  HookTargetType type;

  // We are safe to store a reference since this is ephemeral
  v8::Local<v8::Context> context;

  HookTarget(HookTargetType type, v8::Local<v8::Context> context) {
    this->type = type;
    this->context = context;
  }
};

class Hook {
  /*
   * Each hook represents a patch for a specific JS feature API
   * (e.g. WebGL, Canvas, etc.) A hook instance binds with a
   * specific target (e.g. Worker, Window, etc.)
   */
 public:

  explicit Hook(HookTarget target)
      : signature("f" + std::to_string(rand() % 1000000000000 + 100000000000)),
        target(target) {}
  Hook() = default;
  /*
   * An ephemeral function name that will represent the hook
   * No parameters are defined: in JavaScript, arguments can be referenced via
   * builtin `arguments` variable (e.g. arguments[0], arguments[1], etc.)
   */
  std::string signature;

  HookTarget target;

  // Returns the definition, but not the invocation
  std::string get_definition();


  // Returns immediately-invoked function expression (IIFE)
  std::string get_iife();

  virtual std::string codename();

  /*
   * The implementation [body inner part] of the the hook function definition
   * (aka routine) This is where the actual patch logic is defined by
   * implementors of `Hook` (subclasses)
   */
  virtual std::string get_impl();
};


typedef std::function<std::unique_ptr<Hook>()> HookConstructor;


/*
 * HookFactory is a factory class that creates Hook instances
 * based on a given key (e.g. "webgl", "canvas", etc.)
 * The key is used to lookup the registry of Hook constructors
 * (i.e. HookFactory::GetRegistry())
 * Yeah, I mixed factory & model in the same file, so what?
 */
class HookFactory {
public:

    static void Register(const std::string& key, HookConstructor constructor);
    static std::unique_ptr<Hook> Create(const std::string& key);
    static std::map<std::string, HookConstructor>& GetRegistry();

};
// Macro to define a self-registering hook
#define REGISTER_HOOK(name, type) \
    static bool _hook_##name##_registered = \
        (HookFactory::Register(#name, []{ return std::make_unique<type>(); }), true);


}  // namespace evasions
}  // namespace fingerprinting

#endif  // FINGERPRINTING_EVASIONS_HOOK_H_
