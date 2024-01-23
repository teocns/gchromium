#include "components/fingerprinting/renderer/evasions/hook.h"
#include "fingerprinting/core/evasions/pack.h"
#include "v8-isolate.h"
#include "v8-local-handle.h"
#include "v8-script.h"

// Convert EvasionsPack to v8 script

inline v8::MaybeLocal<v8::UnboundScript> ToV8Script(
    v8::Isolate* isolate,
    fingerprinting::core::evasions::EvasionsPackage* pack) {
  v8::Local<v8::String> source =
      v8::String::NewFromUtf8(isolate, pack->get_iife().c_str())
          .ToLocalChecked();
  return v8::Script::Compile(isolate->GetCurrentContext(), source);
}
