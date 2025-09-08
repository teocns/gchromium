This module is an add-on for fingerprint tampering.


Fingerprints are passed via the `--fingerprint` parameter.

Evasions are "patches" containing parametrized javascript functions that take external fingerprint attributes and tamper the javascript context for the following targets:

#### Point of patch

For pages and iframes: RenderFrameImpl::DidCreateScriptContext passes a blink::ScriptState* argument to its handler. You can get the V8 context from ScriptState using its GetContext() method.

For dedicated workers: In DedicatedWorkerObjectProxy::DidCreateWorkerGlobalScope, you can access the V8 context using WorkerThread::GlobalScope()->ScriptController()->GetScriptState()->GetContext().

For shared workers: In WebSharedWorkerImpl::StartWorkerContext, the context can be obtained from worker_thread_->GlobalScope()->ScriptController()->GetScriptState()->GetContext().

For service workers: In ServiceWorkerVersion::StartWorker, you can access the V8 context from embedded_worker_->worker_thread()->GlobalScope()->ScriptController()->GetScriptState()->GetContext().

For module scripts: In ModulatorImpl::CompileModule, the context can be accessed from GetScriptState()->GetContext().

WorkerScriptFetchInitiator::Start (?)
