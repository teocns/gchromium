
## ?

There are a few things that both worker and window javascript execution contexts share in common, and that is the [`ScriptState`](third_party/blink/renderer/platform/bindings/script_state.cc).





### Implementation plan


1) 

The ScriptState dispatches lifecycle events to a component [RendererResourceCoordinator](third_party/blink/renderer/platform/instrumentation/resource_coordinator/renderer_resource_coordinator.h):

```
ScriptState::ScriptState(v8::Local<v8::Context> context,
                         scoped_refptr<DOMWrapperWorld> world,
                         ExecutionContext* execution_context) {
    ...
  RendererResourceCoordinator::Get()->OnScriptStateCreated(this, execution_context);
    < Add our own event handling here >
}
```




## Registering a new controller

[BlinkInitializer::Initialize](third_party/blink/renderer/controller/blink_initializer.cc) is responsible for bootstrapping the platform controllers on the main thread:


```
void InitializeCommon(Platform* platform, mojo::BinderMap* binders) {
...
RendererResourceCoordinatorImpl::MaybeInitialize();
}
```




Question: OnScriptStateCreated calls can arrive from any thread. Within this method call, we want to request a resource that is persisted in the main thread.
What's the preferred way to synchronously retrieve it, according to chromium docs?



## Data passing: accessing the fingerprint cross-thread



### What mojo says 

Message pipes which don't cross a process boundary are efficient: sent messages are never copied, and a write on one end will synchronously modify the message queue on the other end. When working with generated C++ bindings, for example, the net result is that a Remote on one thread sending a message to a Receiver on another thread (or even the same thread) is effectively a PostTask to the Binding's TaskRunner with the added -- but often small -- costs of serialization, deserialization, validation, and some internal routing logic.




## Blink architecture recap

- platform/
    A collection of lower level features of Blink that are factored out of a monolithic core/. e.g., geometry and graphics utils.
- core/ and modules/
    The implementation of all web-platform features defined in the specs. core/ implements features tightly coupled with DOM. modules/ implements more self-contained features. e.g. webaudio, indexeddb.
    bindings/core/ and bindings/modules/
    Conceptually bindings/core/ is part of core/, and bindings/modules/ is part of modules/. Files that heavily use V8 APIs are put in bindings/{core,modules}.
- controller/
    A set of high-level libraries that use core/ and modules/. e.g., devtools front-end.


