

# Script wrappable

ScriptWrappable provides a way to map from/to C++ DOM implementation to/from
JavaScript object (platform object).  ToV8() converts a ScriptWrappable to
a v8::Object and toScriptWrappable() converts a v8::Object back to
a ScriptWrappable.  v8::Object as platform object is called "wrapper object".
The wrapper object for the main world is stored in ScriptWrappable.  Wrapper
objects for other worlds are stored in DOMDataStore.



# Mojo V8





```
The JavaScript system API exposes the capabilities to create message pipes, data
pipes, shared buffers and watchers. The API is defined using Web IDL. You could
find the IDL files [here](https://cs.chromium.org/chromium/src/third_party/blink/renderer/core/mojo/)
```
Would it help us advance anyhow in our objective?


#include "third_party/blink/renderer/core/mojo/mojo_handle.h"
third_party/blink/renderer/bindings/core/v8/serialization/serialized_script_value.h
third_party/blink/renderer/bindings/core/v8/serialization/v8_script_value_serializer.cc


enum MojoScope {
  // If MojoJS interface broker is provided to MojoBindingContext, refers to
  // that BrowserInterfaceBroker. Otherwise, it refers to the
  // BrowserInterfaceBroker associated with the current execution context.
  // Either a Document or WorkerGlobalScope.
  "context",
  // Refers to the BrowserInterfaceBroker of the current process.
  //
  // Note: A "process" is not a web concept. In some cases the browser process
  // concept of a "site instance" may be useful however there is currently no
  // BrowserInterfaceBroker per site instance.
  "process",
};
