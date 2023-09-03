The `FingerprintManager` is a singleton intended to be accessed statically:

```
class FINGERPRINTING_EXPORT FingerprintManager
    : virtual public fingerprinting::mojom::FingerprintManager {
 public:
  static FingerprintManager* instance(); // returns the singleton instance;
  std::string GetUserAgent();

  void Bind(mojo::PendingReceiver<fingerprinting::mojom::FingerprintManager>
                receiver);

  ...

 private:
  FingerprintManager();

  ~FingerprintManager() override;
  friend class base::NoDestructor<FingerprintManager>;

  mojo::ReceiverSet<fingerprinting::mojom::FingerprintManager> receivers_;
};
```


It is referenced across different browser process modules, for example:
- `//content/browser` at `BrowserMainLoop::CreateStartupTasks()`
  - This is where the `FingerprintManager` singleton is initialized
  - It is only called once, when the browser process launches:
    ```
    StartupTask initialize_fingerprint_manager = base::BindOnce(
      [] {
        fingerprinting::FingerprintManager::instance()->Init();
        return 0;
      });
      ```
- `//chrome/browser/net` `SystemNetworkContextManager::ConfigureDefaultNetworkContextParams`
  - Runs whenever a new `NetworkContext` is created
  - Invokes `components/embedder_support/user_agent_utils.cc:GetUserAgent()`, which is where we implemented our singleton reference:
    ```
    std::string GetUserAgent() {
      ...
      fingerprinting::FingerprintManager::instance()->GetUserAgent(
          &out);
      ...
    }
    ```



Initially this didn't work out accordingly, because although being same process, the static context wasn't shared between modules referencing FingerprintManager, which was declared as a `source_set`, and translates to a static library.


I had to change the build target type of `FingerprintManager` to be a `shared_library` instead of a `source_set` (which would've been a static library).

//fingerprinting BUILD.gn definition:

```
import("//build/config/chrome_build.gni")
import("//mojo/public/tools/bindings/mojom.gni")


config("fingerprinting_public_config") {
  defines = [ "FINGERPRINTING_IMPLEMENTATION" ]
}

shared_library("fingerprinting") {
  sources = [
    "manager.h",
    "manager.cc",
    "export.h",
  ]

  visibility = [ "*" ]

  public_configs = [ ":fingerprinting_public_config" ]

  deps = [
    "//base",
    "//mojo/public/mojom/base",
  ]

  public_deps = [
    ":mojom",
    "//third_party/abseil-cpp:absl",
  ]
}

mojom("mojom") {
  disable_variants = true
  sources = [
    "manager.mojom",
  ]

  visibility = [ "*" ]
  public_deps = [
    "//mojo/public/mojom/base",
    "//third_party/blink/public/mojom:mojom_core",
    "//third_party/blink/public/mojom:mojom_modules",
    "//third_party/blink/public/mojom:mojom_platform",
  ]

  export_class_attribute = "FINGERPRINTING_EXPORT"
  export_define = "FINGERPRINTING_IMPLEMENTATION=1"
  export_header = "fingerprinting/export.h"

}
```



Now, this works as expected when running a release build, because the `fingerprinting` module is a shared library that has its own address space.
