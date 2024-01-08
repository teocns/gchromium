# Architecture 

https://www.chromium.org/developers/design-documents/displaying-a-web-page-in-chrome/


# V8 Caching

third_party/blink/renderer/bindings/core/v8/v8_code_cache.h


Cache at a higher scope - like in the Document/Frame that created the worker:
When the document creates a worker, check if the data is cached
If not, fetch it and cache on the document
Pass the cached data into WorkerThread::InitializeOnWorkerThread()
Use a global cache shared by all workers:
Have a static cache accessible from all worker threads
WorkerThread checks this cache on init and populates if needed
May need locks/synchronization to avoid races
Cache per-Worker object:
The Worker object creating the thread persists between threads
Could store cache as a member of Worker
Passed into InitializeOnWorkerThread() when new thread is created

# WebV8Features

std::unique_ptr<v8::ScriptCompiler::CachedData> V8CodeCache::CreateCachedData;


// WebV8Features is used in conjunction with IDL interface features which
// specify a [ContextEnabled] extended attribute. Such features may be enabled
// for arbitrary main-world V8 contexts by using these methods during
// WebLocalFrameClient::DidCreateScriptContext. Enabling a given feature causes
// its binding(s) to be installed on the appropriate global object(s) during
// context initialization.
//
// See src/third_party/blink/renderer/bindings/IDLExtendedAttributes.md for more
// information.

class BLINK_EXPORT WebV8Features {
 public:
  static void EnableMojoJS(v8::Local<v8::Context>, bool);

  static void EnableMojoJSAndUseBroker(
      v8::Local<v8::Context> context,
      mojo::PendingRemote<blink::mojom::BrowserInterfaceBroker> broker_remote);

  static void EnableMojoJSFileSystemAccessHelper(v8::Local<v8::Context>, bool);

 private:
  WebV8Features() = delete;
};

# Mojo V8

The JavaScript system API exposes the capabilities to create message pipes, data
pipes, shared buffers and watchers. The API is defined using Web IDL. You could
find the IDL files [here](https://cs.chromium.org/chromium/src/third_party/blink/renderer/core/mojo/)



# V8 Wrapping


## How to retain an ECMAScript value (v8::Value) in a Blink object?

In general, ECMAScript values (v8::Values) are associated with a realm
(v8::Context) and must be protected against a leak across v8::Context and/or
isolated worlds.  You must be extra careful when storing a v8::Value in a Blink
object.

The best way is to avoid storing a v8::Value in Blink, however, if you end up
storing a v8::Value in a Blink object, there are two options;
`TraceWrapperV8Reference` and `WorldSafeWrapperReference`.

`TraceWrapperV8Reference<V8Type>` works just like Member<BlinkType> and makes the
V8Type value alive as long as it's traced via `Trace` member function.  If
you're pretty sure that the value never be accessible across isolated worlds,
this is the default choice.

`WorldSafeWrapperReference<V8Type>` is recommended if the value is accessible
across isolated worlds and/or if there are any security concerns.  Compared to
TraceWrapperV8Reference, WorldSafeWrapperReference provides extra checks against
cross-world access and object-cloning across isolated worlds.
WorldSafeWrapperReference doesn't prevent information leak, but it prevents world
leak.  An example use case is Event.  Events are often dispatched not only in
the main world but also in isolated worlds, too.  Event objects may be
accessible in a variety of worlds, so it's a good choice to use
WorldSafeWrapperReference to store a v8::Value in an Event object.

`ScriptValue`, `v8::Persistent`, `v8::Global`, and `v8::Eternal` are _NOT_
recommended as a way of storing v8::Value in a Blink object.  They had been used
in the past, but they're now obsolete after the unified heap project.  Now
TraceWrapperV8Reference and WorldSafeWrapperReference are recommended.


