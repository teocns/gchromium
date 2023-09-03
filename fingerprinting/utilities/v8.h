

#ifndef FINGERPRINTING_UTILITIES_V8_H_
#define FINGERPRINTING_UTILITIES_V8_H_

#include "v8/include/v8.h"

#include <set>
#include <string>
#include "fingerprinting/utilities/export.h"

namespace fingerprinting {

namespace utilities {

FINGERPRINTING_UTILITY_EXPORT void patchWrapper(const v8::FunctionCallbackInfo<v8::Value>& innerArgs);

FINGERPRINTING_UTILITY_EXPORT void ThrowAndLogError(v8::Isolate* isolate, const char* message);

FINGERPRINTING_UTILITY_EXPORT void CopyOwnPropertiesDescriptors(v8::Local<v8::Object> source,
                                  v8::Local<v8::Object> target,
                                  v8::Local<v8::Context> context,
                                  v8::Isolate* isolate);

FINGERPRINTING_UTILITY_EXPORT void PatchAccessor(const v8::FunctionCallbackInfo<v8::Value>& args);

FINGERPRINTING_UTILITY_EXPORT void PatchValue(const v8::FunctionCallbackInfo<v8::Value>& args);

}  // namespace utilities

}  // namespace fingerprinting

#endif  // FINGERPRINTING_UTILITIES_V8_H_
