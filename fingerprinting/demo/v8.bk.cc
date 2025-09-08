
// Copyright 2015 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include "v8/include/v8.h"
#include <vector>
#include <string>
#include "libplatform/libplatform.h"
#include <format>

void PutVariable(v8::Isolate *isolate, v8::Local<v8::Context> context,
                 const char *name, v8::Local<v8::Value> value) {
  v8::Local<v8::String> key =
      v8::String::NewFromUtf8(isolate, name).ToLocalChecked();
  context->Global()->Set(context, key, value).Check();
}

bool HasVariable(v8::Isolate *isolate, v8::Local<v8::Context> context,
                 const char *name) {
  v8::Local<v8::String> key =
      v8::String::NewFromUtf8(isolate, name).ToLocalChecked();
  return context->Global()->Has(context, key).FromJust();
}

// args[0] = v8::FunctionTemplate::New(isolate,
// fingerprinting::evasions::utility::PatchAccessor)
//               ->GetFunction(context)
//               .ToLocalChecked();
//
// args[1] = v8::FunctionTemplate::New(isolate,
// fingerprinting::evasions::utility:PatchAccessor)
//               ->GetFunction(context)
//               .ToLocalChecked();
void DummyFunction(const v8::FunctionCallbackInfo<v8::Value> &args) {
  // Dummy function that just returns strilg "dummy"

  v8::Isolate *isolate = args.GetIsolate();
  v8::HandleScope handle_scope(isolate);
  v8::Local<v8::String> str =
      v8::String::NewFromUtf8(isolate, "dummy").ToLocalChecked();
  args.GetReturnValue().Set(str);
}

v8::MaybeLocal<v8::Function> GetFunction(
    v8::Local<v8::Context> context, std::string hook_impl) {
  v8::Isolate* isolate = context->GetIsolate();

  // Create an IIFE
  v8::Local<v8::String> js_src;
  if (!v8::String::NewFromUtf8(
           isolate,
           std::format("(function (){{ {} }})", hook_impl).c_str(),
           v8::NewStringType::kNormal)
           .ToLocal(&js_src)) {
    // isolate->ThrowException(v8::Exception::Error(
    //     v8::String::NewFromUtf8(isolate, "Failed to create JS source
    //     string").ToLocalChecked()));
    return v8::MaybeLocal<v8::Function>();
  }

  v8::Local<v8::Script> script;
  if (!v8::Script::Compile(context, js_src).ToLocal(&script)) {
    // isolate->ThrowException(v8::Exception::Error(
    //     v8::String::NewFromUtf8(isolate, "Failed to compile
    //     script").ToLocalChecked()));
    return v8::MaybeLocal<v8::Function>();
  }

  v8::Local<v8::Value> maybeFunction;
  if (!script->Run(context).ToLocal(&maybeFunction)) {
    // isolate->ThrowException(v8::Exception::Error(
    //     v8::String::NewFromUtf8(isolate, "Failed to run
    //     script").ToLocalChecked()));
    return v8::MaybeLocal<v8::Function>();
  }

  return v8::Local<v8::Function>::Cast(maybeFunction);
}
void RunManaged(v8::Isolate *isolate, v8::Local<v8::Context> context,
                std::string src) {
  // Run managed code in the context of the isolate.
  // We try-catch the code to avoid crashing the isolate.

  v8::TryCatch try_catch(isolate);
  auto source = v8::String::NewFromUtf8(isolate, src.c_str()).ToLocalChecked();
  auto script = v8::Script::Compile(context, source).ToLocalChecked();
  auto result = script->Run(context).ToLocalChecked();
  v8::String::Utf8Value utf8(isolate, result);
  printf("%s\n", *utf8);
  if (try_catch.HasCaught()) {
    printf("Error\n");
  }
}



const std::vector<std::string> scripts = {
    "return arguments",
    "console.log('Bye, World!');",
    "return globalThis['asdf']",
};

int main(int argc, char *argv[]) {
  // Initialize V8.
  v8::V8::InitializeICUDefaultLocation(argv[0]);
  v8::V8::InitializeExternalStartupData(argv[0]);
  std::unique_ptr<v8::Platform> platform = v8::platform::NewDefaultPlatform();
  v8::V8::InitializePlatform(platform.get());
  v8::V8::Initialize();
  // Create a new Isolate and make it the current one.
  v8::Isolate::CreateParams create_params;
  create_params.array_buffer_allocator =
      v8::ArrayBuffer::Allocator::NewDefaultAllocator();
  v8::Isolate *isolate = v8::Isolate::New(create_params);

  {
    v8::Isolate::Scope isolate_scope(isolate);
    // Create a stack-allocated handle scope.
    v8::HandleScope handle_scope(isolate);
    // Create a new context.
    v8::Local<v8::Context> context = v8::Context::New(isolate);
    // Enter the context for compiling and running the hello world script.
    v8::Context::Scope context_scope(context);


    v8::Local<v8::FunctionTemplate> dummy_function_template =
        v8::FunctionTemplate::New(isolate, DummyFunction);

    v8::Local<v8::Function> dummy_function =
        dummy_function_template->GetFunction(context).ToLocalChecked();



    // v8::Persistent<v8::Function> dummy_function_persistent(isolate, dummy_function);

    for (auto script_str : scripts) {
      {
        // Create a string containing the JavaScript source code.
        // v8::Local<v8::String> source =
        //     v8::String::NewFromUtf8Literal(isolate, script_str.c_str());
        // // Compile the source code.
        // v8::Local<v8::Script> script =
        //     v8::Script::Compile(context, source).ToLocalChecked();
        // // Run the script to get the result.
        // v8::Local<v8::Value> result = script->Run(context).ToLocalChecked();
        // // Convert the result to an UTF8 string and print it.
        // v8::String::Utf8Value utf8(isolate, result);
        //
        v8::TryCatch try_catch(isolate);
        v8::Local<v8::Function> fun = GetFunction(context, script_str).ToLocalChecked();


        // Invoke the function with dummy_function_persistent as argument (must convert to Local first)
        
        v8::Local<v8::Value> args[1] = {dummy_function};

        v8::MaybeLocal<v8::Value> ret = fun->Call(context, context->Global(), 1, args).ToLocalChecked();
        // Attempt to "stringify" the return value 

        if (try_catch.HasCaught()) {
          printf("Error: %s\n", *v8::String::Utf8Value(isolate, try_catch.Exception()));
        }
        if (!ret.IsEmpty()) {
          v8::String::Utf8Value utf8(isolate, ret.ToLocalChecked());
          printf("%s\n", *utf8);
        }


        // RunManaged(isolate, context, script_str);
      }
    }
  }
  // Dispose the isolate and tear down V8.
  isolate->Dispose();
  v8::V8::Dispose();
  v8::V8::DisposePlatform();
  delete create_params.array_buffer_allocator;
  return 0;
}
