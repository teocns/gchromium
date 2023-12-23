

#ifndef FINGERPRINTING_PUBLIC_EVASIONS_V8UTIL_H
#define FINGERPRINTING_PUBLIC_EVASIONS_V8UTIL_H

#include "base/component_export.h"
#include "v8/include/v8.h"

#include <set>
#include <string>

namespace fingerprinting::evasions::utility {

void patchWrapper(const v8::FunctionCallbackInfo<v8::Value>& innerArgs);

void ThrowAndLogError(v8::Isolate* isolate, const char* message);

void CopyOwnPropertiesDescriptors(v8::Local<v8::Object> source,
                                  v8::Local<v8::Object> target,
                                  v8::Local<v8::Context> context,
                                  v8::Isolate* isolate);

COMPONENT_EXPORT(FINGERPRINTING_PUBLIC_EVASIONS)
void PatchAccessor(const v8::FunctionCallbackInfo<v8::Value>& args);

COMPONENT_EXPORT(FINGERPRINTING_PUBLIC_EVASIONS)
void PatchValue(const v8::FunctionCallbackInfo<v8::Value>& args);

COMPONENT_EXPORT(FINGERPRINTING_PUBLIC_EVASIONS)
void RunWithUtils(v8::Local<v8::Context> context, std::string source_code);

}  // namespace fingerprinting::evasions::utility

#endif  // FINGERPRINTING_PUBLIC_EVASIONS_V8UTIL_H
