Ghost (GChromium) browser implementation.



# ContentClient

The entrypoint for the browser content is (ContentClient)[content/public/common/content_client.h], an Interface that the embedder implements. It exposes the following clients:

```
ContentBrowserClient* browser() { return browser_; }
ContentGpuClient* gpu() { return gpu_; }
ContentRendererClient* renderer() { return renderer_; }
ContentUtilityClient* utility() { return utility_; }
```

A (`GhostMainDelegate`)[ghost/app/ghost_main_delgate.h] overrides (`ChromeMainDelegate`)[chrome/app/main_delegate.h] in `ChromeMain`. 

are the "core" of the browser, and they are responsible for managing lifecycle of the various components observers

Functional components that serve as handlers are:

- (`RenderThreadObserver`)[content/public/renderer/render_thread_observer.h]
- (`RenderFrameObserver`)[content/public/renderer/render_frame_observer.h]

## Thinking multi-process and multi-threaded


[Browser process IPC diagram](ghost/docs/browser_renderer_ipc.png)

Blink has one main thread, N worker threads and a couple of internal threads. This means that all the important stuff happens on the main thread, hence assuming a mostly single-threaded architecture.


### Renderer threads: 2 kinds

Blink runs on a singleyy _Renderer_ thread, but it also interfaces cross-thread with workers belonging to the same (renderer process)[https://www.chromium.org/developers/design-documents/displaying-a-web-page-in-chrome/#the-render-process].

> There is one renderer [main] thread and N worker threads within one Renderer process.

The renderer flow begins with (RendererProcess::RenderMain)[content/renderer/renderer_main.cc], mainline routine for running as the Renderer process: it initializes *blink::Platform* and `blink::scheduler::WebThreadScheduler`.






## RenderThread

The (`RenderThreadImpl`)[content/renderer/render_thread_impl.h] class represents the main thread, where `blink::WebView` instances live. 


# RendererBlinkPlatform

(RendererBlinkPlatformImpl::WorkerContextCreated)[content/renderer/renderer_blink_platform_impl.cc]


```
void RendererBlinkPlatformImpl::WorkerContextCreated(
    const v8::Local<v8::Context>& worker) {
  GetContentClient()->renderer()->DidInitializeWorkerContextOnWorkerThread(
      worker);
}

```
whereas `renderer()` is an instance of (`ContentRendererClient`)[content/public/renderer/content_renderer_client.h]




# Cross-thread resource sharing

### Brainstorms 
Use the BrowserContext's StoragePartition to store/retrieve data via DOMStorage or IndexedDB APIs. This data is segregated by origin.
Use a KeyedService in the browser process to store the data more globally.

Mojo interfaces can be used to pass data between browser and renderer.
The renderer could query the storage partition or KeyedService when needed.
For read-only data, a global singleton could be initialized in the renderer using browser-side IPC.
RenderFrameObserver is useful for reacting to render frame lifecycle events. But the data storage and IPC should happen separately.

Threading and sequences are critical - objects must be accessed on the right thread/sequence.

RendererResourceCoordinator is a process-wide singleton, and thread-safe.

[DOM Distiller](components/dom_distiller/content/renderer/distiller_native_javascript.cc) provides an interface for injecting JavaScript on frames. Callbacks for JavaScript functionality and the communication channel to the browser process are maintained here.




# The actual injection



The [WebLocalFrame](third_party/blink/renderer/core/frame/web_local_frame_impl.cc) provides functionalities for exeuting scripts:

`
render_frame()->GetWebFrame()->CallFunctionEvenIfScriptDisabled( callback_local, v8::Object::New(isolate), 2, argv);
`


HOWEVER! 
We are not interested because this only works for frames. And workers?

