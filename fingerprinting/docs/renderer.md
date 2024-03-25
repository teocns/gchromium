# content/renderer and blink/renderer
The relationship and workflow between the content/renderer and blink/renderer modules in Chromium is:

The content/renderer module handles high-level browser logic like navigation, security, and multi-process communication. It manages RenderFrameHosts which host RenderFrames.

The blink/renderer module implements the bulk of rendering logic via the Blink rendering engine. It has RenderFrames which manage Documents and handle parsing, script execution, layout, painting etc.

Each RenderFrameHost in content/renderer hosts a RenderFrame in blink/renderer. The RenderFrameHost delegates most rendering logic to its associated RenderFrame.

RenderFrames can create worker threads like WorkerThread for JavaScript Web Workers. These WorkerThreads similarly delegate to a WorkerGlobalScope in Blink.

### Summary 
content/renderer handles browser logic and lifetimes of RenderFrameHosts
blink/renderer handles document rendering via RenderFrames
RenderFrameHosts host and delegate to RenderFrames
RenderFrames can create WorkerThreads that delegate to WorkerGlobalScopes

### Chain of responsibility

content/renderer → blink/renderer → (WorkerThreads → blink/renderer)
With content/renderer managing lifetimes and blink/renderer handling most rendering logic.
The render process is created by the browser process and handles rendering web content. Its main components are:

RenderProcessHostImpl - Manages the lifetime of the render process.
RenderThread - The main thread in the render process. Handles IPC and initializes components like Blink.
RenderFrames - Manage documents, parsing, script execution etc. Created on the RenderThread.


# Fingerprint injection 
The `Fingerprint` is persisted in the browser process. Workers and Frames' runtimes run in the `RenderProcess` fetched via an IPC call to a mojom interface bound with to the browser process
The objective is to persist the `Fingerprint` object, , into a V8 object that can be shared concurrently with N RenderThreads. Given these considerations:
