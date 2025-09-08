# Fingerpinting injection: multi-process passthrough

## Objective

- Persist the `Fingerprint` object in the `RenderProcess` as a V8 object.
- Share concurrently with N `RenderThreads`.
- Aim to minimize the serialization overhead.

# Knowledge

### Content/Renderer and Blink/Renderer in Chromium
- **Purpose**: Explaining the relationship and workflow between the `content/renderer` and `blink/renderer` modules in Chromium.
- **Key Components**:
  - `content/renderer`: Manages high-level browser logic (navigation, security, multi-process communication).
  - `blink/renderer`: Implements the bulk of rendering logic via the Blink rendering engine.

### Detailed Workflow
1. **content/renderer**:
   - Manages RenderFrameHosts which host RenderFrames.
   - Handles browser logic and lifetimes of RenderFrameHosts.
2. **blink/renderer**:
   - Manages RenderFrames for document rendering.
   - Handles parsing, script execution, layout, painting, etc.
   - RenderFrames can create WorkerThreads.

### Interaction
- RenderFrameHost in `content/renderer` hosts a RenderFrame in `blink/renderer`.
- The RenderFrameHost delegates most rendering logic to its associated RenderFrame.
- WorkerThreads in RenderFrames delegate to WorkerGlobalScopes in Blink.
- **Chain of Responsibility**: `content/renderer → blink/renderer → (WorkerThreads → blink/renderer)`

### Render Process Components
- **RenderProcessHostImpl**: Manages the lifetime of the render process.
- **RenderThread**: Main thread in the render process, handles IPC, initializes components like Blink.
- **RenderFrames**: Manages documents, parsing, script execution, etc.


# Implementation

### Must-know
- Does the RenderProcess have context over the v8::Isolate/world to persist such an object on the V8 heap?

```
Each RenderProcess has its own V8 isolate.

The RenderProcess creates v8::Contexts for each RenderFrame it contains.

These v8::Contexts are associated with a DOMWrapperWorld that controls the JavaScript security boundaries.
```


### Approach
- Persist `Fingerprint` in the browser process.
- Fetch `Fingerprint` via an IPC call to a mojom interface bound to the browser process.
- Ensure compatibility with Workers and Frames' runtimes in the `RenderProcess`.

