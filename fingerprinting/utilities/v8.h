

#ifndef FINGERPRINTING_UTILITIES_V8_H_
#define FINGERPRINTING_UTILITIES_V8_H_

#include "v8/include/v8.h"

#include <set>
#include <string>
#include "fingerprinting/utilities/export.h"

namespace fingerprinting {

namespace utilities {

namespace v8_patcher {

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

FINGERPRINTING_UTILITY_EXPORT void RunWithUtils(v8::Local<v8::Context> context, std::string source_code);

}  // namespace v8_patcher
}  // namespace utilities

}  // namespace fingerprinting

#endif  // FINGERPRINTING_UTILITIES_V8_H_
