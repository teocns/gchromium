Implementation for the fingerprinting JavaScript context mock

This controller establishes communications with the browser process to retrieve the fingerprint. 




# Mojo Interface


the objective is to expose an interface accessible to blink/renderer/controller to make it immediately accessible to the render process after blink initializes with GetBlinkInitializer().RegisterInterfaces(*binders)[third_party/blink/renderer/controller/blink_initializer.cc]



There, we should be able to do something like:
```
  platform->GetBrowserInterfaceBroker()->GetInterface(
      fingerprint_manager.BindNewPipeAndPassReceiver());
```


The documentation suggests to expose the interface from the browser process by adding an "AddUIThreadInterface" at content/browser/renderer_host/render_process_host_impl.cc, however, we do not want to touch the content layer. Since we have implemented our own embedder in //chrome, how can we achieve the same result from within the embedder?
