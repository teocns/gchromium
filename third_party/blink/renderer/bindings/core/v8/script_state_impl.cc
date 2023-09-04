// Copyright 2023 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/bindings/core/v8/script_state_impl.h"

#include "third_party/blink/renderer/core/execution_context/execution_context.h"
#include "third_party/blink/renderer/platform/bindings/dom_wrapper_world.h"
#include "third_party/blink/public/web/blink.h"
#include "fingerprinting/utilities/v8.h"
#include "v8/include/v8-isolate.h"
#include "v8/include/v8-local-handle.h"
#include "v8/include/v8-microtask-queue.h"

namespace blink {

// static
void ScriptStateImpl::Init() {
  ScriptState::SetCreateCallback(ScriptStateImpl::Create);
}

// static
ScriptState* ScriptStateImpl::Create(v8::Local<v8::Context> context,
                                     scoped_refptr<DOMWrapperWorld> world,
                                     ExecutionContext* execution_context) {
  ScriptState* scriptState = MakeGarbageCollected<ScriptStateImpl>(context, std::move(world),
                                               execution_context);



  // For all targets where external source scripts can be executed
  if (
    // If execution context points to 0x0, then it's a detached context
    execution_context != nullptr &&
    (
      execution_context->IsDedicatedWorkerGlobalScope() ||
      execution_context->IsServiceWorkerGlobalScope() ||
      execution_context->IsSharedWorkerGlobalScope() ||
      execution_context->IsWorkerGlobalScope() ||
      execution_context->IsWindow()
    )
  ) {
    // Get context's global object

    std::string executionContextType;
    if (execution_context->IsDedicatedWorkerGlobalScope()) {
      executionContextType = "DedicatedWorkerGlobalScope";
    } else if (execution_context->IsServiceWorkerGlobalScope()) {
      executionContextType = "ServiceWorkerGlobalScope";
    } else if (execution_context->IsSharedWorkerGlobalScope()) {
      executionContextType = "SharedWorkerGlobalScope";
    } else if (execution_context->IsWorkerGlobalScope()) {
      executionContextType = "WorkerGlobalScope";
    } else if (execution_context->IsWindow()) {
      executionContextType = "Window";
    }


    v8::MicrotasksScope microtasks(blink::MainThreadIsolate(),
                                 context->GetMicrotaskQueue(),
                                 v8::MicrotasksScope::kDoNotRunMicrotasks);

    // Expose replaceHandler
    v8::Isolate* isolate = context->GetIsolate();
    v8::HandleScope handle_scope(isolate);
    // v8::MicrotasksScope microtask_scope(isolate, v8::MicrotasksScope::Type::kRunMicrotasks);
    v8::Context::Scope context_scope(context);

    // Create a new function template
    v8::Local<v8::FunctionTemplate> patch_accessor_func_template = v8::FunctionTemplate::New(isolate, fingerprinting::utilities::PatchAccessor);
    v8::Local<v8::FunctionTemplate> patch_internal_method_func_template = v8::FunctionTemplate::New(isolate, fingerprinting::utilities::PatchValue);

    // Get the global object
    v8::Local<v8::Object> global = context->Global();

    // Inject the function into the global objecta
    global->Set(context, v8::String::NewFromUtf8(isolate, "PatchAccessor").ToLocalChecked(), patch_accessor_func_template->GetFunction(context).ToLocalChecked()).FromJust();
    global->Set(context, v8::String::NewFromUtf8(isolate, "PatchValue").ToLocalChecked(), patch_internal_method_func_template->GetFunction(context).ToLocalChecked()).FromJust();
  }
  return scriptState;
}

ScriptStateImpl::ScriptStateImpl(v8::Local<v8::Context> context,
                                 scoped_refptr<DOMWrapperWorld> world,
                                 ExecutionContext* execution_context)
    : ScriptState(context, std::move(world), execution_context),
      execution_context_(execution_context) {}

void ScriptStateImpl::Trace(Visitor* visitor) const {
  ScriptState::Trace(visitor);
  visitor->Trace(execution_context_);
}

}  // namespace blink
