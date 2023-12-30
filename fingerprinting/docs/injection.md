To patch anything that runs a JavaScript context (worker, frame, page, etc.), the best place to do so would be the bindings/ directory, as this is where files that use V8 APIs heavily reside.

When you need to fetch V8 patches via a Mojo receiver that communicates with the browser process, Blink can use Mojo directly to talk to the browser process【37†source】. The specific binding location isn't explicitly mentioned in the provided documentation, but in general, blinks are linked to Mojo communication systems for such interactions.

The V8 native functions and C++ APIs that you want to inject into the V8 context should also be stored in the bindings/ directory, as this is the part of the architecture that deals with V8 API usage.

Execution logic or a manager that orchestrates such a patch should be located in the controller/ directory if its sole purpose is to drive the Web Platform or to implement an API for the embedder【45†source】. The invocation point is not detailed in the documentation, so the standard practice would typically involve invoking the manager where it has the necessary access to the V8 context and JavaScript execution, which might be within the core/ or related subdirectories like bindings/core.




We are looking for the right place to patch global objects (Navigator, CSSStyleDeclaration, etc) of a JavaScript context (worker, frame, page, etc.).

As a first step, we need to understand how these properties get created; the architecture is quite complex, and I am questioning myself about the relationship and workflow of frame, local frames, workers, etc- contexts.

We're ideally looking for an unique spot where we can catch the creation of said contexts and patch them before external scripts can run into it.


# Global object proxy association

It happens in:

- WorkerOrWorkletScriptController::Initialize

third_party/blink/renderer/bindings/core/v8/local_window_proxy.cc



# Navigator properties exposure:

third_party/blink/renderer/core/workers/worker_global_scope.cc

third_party/blink/renderer/core/frame/local_dom_window.cc




# About ScriptState


Every script context creation goes through the following method:

ScriptState* ScriptStateImpl::Create(v8::Local<v8::Context> context,
                                     scoped_refptr<DOMWrapperWorld> world,
                                     ExecutionContext* execution_context) {
  return MakeGarbageCollected<ScriptStateImpl>(context, std::move(world),
                                               execution_context);
}

We are dealing with either frames or workers. So what's the correlation between a DOMWrapperWorld and a worker?



## Frame

 blink::ScriptStateImpl::Create(v8::Local<v8::Context>, scoped_refptr<blink::DOMWrapperWorld>, blink::ExecutionContext*) script_state_impl.cc:21
 blink::ScriptState::Create(v8::Local<v8::Context>, scoped_refptr<blink::DOMWrapperWorld>, blink::ExecutionContext*) script_state.cc:27
 blink::LocalWindowProxy::CreateContext() local_window_proxy.cc:251
 blink::LocalWindowProxy::Initialize() local_window_proxy.cc:149
 blink::LocalWindowProxy::UpdateDocument() local_window_proxy.cc:423
 blink::LocalWindowProxyManager::UpdateDocument() window_proxy_manager.cc:136
 blink::ScriptController::UpdateDocument() script_controller.cc:229
 blink::DocumentLoader::CreateParserPostCommit() document_loader.cc:2827
 blink::DocumentLoader::StartLoadingResponse() document_loader.cc:1784
 blink::DocumentLoader::CommitNavigation() document_loader.cc:2733
 blink::FrameLoader::CommitDocumentLoader(blink::DocumentLoader*, blink::HistoryItem*, blink::CommitReason) frame_loader.cc:1352
 blink::FrameLoader::CommitNavigation(std::__Cr::unique_ptr<blink::WebNavigationParams, std::__Cr::default_delete<blink::WebNavigationParams>>, std::__Cr::unique_ptr<blink::WebDocumentLoader::ExtraData, std::__Cr::default_delete<blink::WebDocumentLoader::ExtraData>>, blink::CommitReason) frame_loader.cc:1173
 blink::WebLocalFrameImpl::CommitNavigation(std::__Cr::unique_ptr<blink::WebNavigationParams, std::__Cr::default_delete<blink::WebNavigationParams>>, std::__Cr::unique_ptr<blink::WebDocumentLoader::ExtraData, std::__Cr::default_delete<blink::WebDocumentLoader::ExtraData>>) web_local_frame_impl.cc:2772


## Worker


 blink::ScriptStateImpl::Create(v8::Local<v8::Context>, scoped_refptr<blink::DOMWrapperWorld>, blink::ExecutionContext*) script_state_impl.cc:21
 blink::ScriptState::Create(v8::Local<v8::Context>, scoped_refptr<blink::DOMWrapperWorld>, blink::ExecutionContext*) script_state.cc:27
 blink::WorkerOrWorkletScriptController::Initialize(blink::KURL const&) worker_or_worklet_script_controller.cc:144
 blink::WorkerThread::InitializeOnWorkerThread(std::__Cr::unique_ptr<blink::GlobalScopeCreationParams, std::__Cr::default_delete<blink::GlobalScopeCreationParams>>, absl::optional<blink::WorkerBackingThreadStartupData> const&, std::__Cr::unique_ptr<blink::WorkerDevToolsParams, std::__Cr::default_delete<blink::WorkerDevToolsParams>>) worker_thread.cc:660


# Caching

`third_party/blink/renderer/platform/bindings/v8_value_cache.h` 

// String cache helps convert WTF strings (StringImpl*) into v8 strings by
// only creating a v8::String for a particular StringImpl* once and caching it
// for future use. It is held by and can be retrieved from V8PerIsolateData, and
// is cleared when the isolate is destroyed. Entries are removed from the
// backing global value map when weak references to the values are collected.



 content::RenderFrameImpl::DidCreateScriptContext(v8::Local<v8::Context>, int) render_frame_impl.cc:4417
 blink::LocalFrameClientImpl::DidCreateScriptContext(v8::Local<v8::Context>, int) local_frame_client_impl.cc:283
 blink::LocalWindowProxy::Initialize() local_window_proxy.cc:197
 blink::WindowProxy::InitializeIfNeeded() window_proxy.cc:155
 blink::WindowProxyManager::GetWindowProxy(blink::DOMWrapperWorld&) window_proxy_manager.h:47
 blink::Frame::GetWindowProxy(blink::DOMWrapperWorld&) frame.cc:273
 blink::LocalFrame::WindowProxy(blink::DOMWrapperWorld&) local_frame.cc:962
 blink::ToV8ContextEvenIfDetached(blink::LocalFrame*, blink::DOMWrapperWorld&) v8_binding_for_core.cc:740
 blink::ToScriptStateImpl(blink::LocalFrame*, blink::DOMWrapperWorld&) v8_binding_for_core.cc:692
 blink::ToScriptState(blink::LocalFrame*, blink::DOMWrapperWorld&) v8_binding_for_core.cc:781
 blink::ToScriptStateForMainWorld(blink::LocalFrame*) v8_binding_for_core.cc:785
 blink::WebLocalFrameImpl::MainWorldScriptContext() const web_local_frame_impl.cc:1180
 content::WebUIExtension::Install(blink::WebLocalFrame*) web_ui_extension.cc:92
 content::RenderFrameImpl::DidClearWindowObject() render_frame_impl.cc:3865
 blink::LocalFrameClientImpl::DispatchDidClearWindowObjectInMainWorld(v8::Isolate*, v8::MicrotaskQueue*) local_frame_client_impl.cc:205
 blink::FrameLoader::DispatchDidClearDocumentOfWindowObject() frame_loader.cc:1686
 blink::DocumentLoader::CreateParserPostCommit() document_loader.cc:2829



