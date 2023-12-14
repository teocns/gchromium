

#ifndef FINGERPRINTING_UTILITY_V8_H_
#define FINGERPRINTING_UTILITY_V8_H_

#include "base/component_export.h"
#include "v8/include/v8.h"

#include <set>
#include <string>

namespace fingerprinting::utility{

void patchWrapper(const v8::FunctionCallbackInfo<v8::Value>& innerArgs);

void ThrowAndLogError(v8::Isolate* isolate, const char* message);

void CopyOwnPropertiesDescriptors(v8::Local<v8::Object> source,
                                  v8::Local<v8::Object> target,
                                  v8::Local<v8::Context> context,
                                  v8::Isolate* isolate);

void PatchAccessor(const v8::FunctionCallbackInfo<v8::Value>& args);

void PatchValue(const v8::FunctionCallbackInfo<v8::Value>& args);

class ExecutionContext {
 public:
  ExecutionContext(v8::Local<v8::Context> context, v8::Isolate* isolate)
      : context(context), isolate(isolate) {}

  v8::Local<v8::Context> context;
  v8::Isolate* isolate;
};

COMPONENT_EXPORT(FINGERPRINTING_UTILITY_V8)
void RunWithUtils(v8::Local<v8::Context> context, std::string source_code);

}  // namespace fingerprinting::utility::v8_patcher

#endif  // FINGERPRINTING_UTILITY_V8_H_
