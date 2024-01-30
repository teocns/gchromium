#include "third_party/blink/renderer/extensions/ghost/fingerprinting.h"

#include "base/logging.h"
#include "base/process/current_process.h"
#include "base/process/process.h"
#include "base/time/time.h"
#include "base/values.h"
#include "components/fingerprinting/renderer/evasions/hook_execution_context.h"
#include "components/fingerprinting/renderer/evasions/pack.h"
#include "components/fingerprinting/renderer/evasions/package_execution_context.h"
#include "fingerprinting/core/device_descriptor/fingerprint_impl.h"
#include "fingerprinting/public/cpp/manager.h"
#include "fingerprinting/public/mojom/manager.mojom.h"
#include "third_party/blink/public/common/thread_safe_browser_interface_broker_proxy.h"
#include "third_party/blink/public/mojom/browser_interface_broker.mojom-blink.h"
#include "third_party/blink/public/platform/platform.h"
#include "third_party/blink/renderer/core/execution_context/execution_context.h"
#include "third_party/blink/renderer/platform/bindings/extensions_registry.h"
#include "third_party/blink/renderer/platform/bindings/v8_set_return_value.h"
#include "third_party/blink/renderer/platform/fingerprinting/provider.h"
#include "third_party/blink/renderer/platform/weborigin/scheme_registry.h"
#include "third_party/blink/renderer/platform/weborigin/security_origin.h"
#include "url/url_constants.h"

namespace blink {

using fingerprinting::core::evasions::EvasionsPackage;
using fingerprinting::core::evasions::Hook;
using fingerprinting::core::evasions::HookTargetType;
using fingerprinting::evasions::EvasionsPackageExecutionContext;
using fingerprinting::evasions::HookExecutionContext;
void InstallFingerprintingExtensions(ScriptState* script_state) {
  auto* execution_context = ExecutionContext::From(script_state);

  // The acutal patch logic to be executed here....

  // auto global_proxy = script_state->GetContext()->Global();

  LOG(INFO) << "InstallFingerprintExtensions executing()";

  // Create an evasion pack
  // ExecutionContext* exec = ExecutionContext::From(script_state);
  //
  fingerprinting::Fingerprint* fp = FingerprintProvider::Get();

  if (fp == nullptr) {
    LOG(ERROR) << "InstallFingerprintExtensions: NO FINGERPRINT";
    return;
  }

  // Avoid blocking fingerprinting in WebUI, extensions, etc.
  const String protocol = execution_context->GetSecurityOrigin()->Protocol();
  LOG(INFO) << "InstallFingerprintExtensions: Protocol: " << protocol;

  if (protocol == url::kAboutScheme || protocol == "chrome-extension" ||
      blink::SchemeRegistry::ShouldTreatURLSchemeAsDisplayIsolated(protocol)) {
    return;
  }

  // Set the fingerprint json as a persistent data within the isolate
  std::string str = fp->str_value();

  HookTargetType hType =
      HookExecutionContext::TypeFromExecutionContext(execution_context);

  std::unique_ptr<EvasionsPackage> o_pack = EvasionsPackage::Pack(hType);

  EvasionsPackageExecutionContext runner =
      EvasionsPackageExecutionContext(o_pack.get(), script_state, fp);

  runner.Run();

  // EvasionsPackageExecutionContext

  // global_proxy
  //     ->SetLazyDataProperty(script_state->GetContext(),
  //                           V8String(script_state->GetIsolate(),
  //                           "chromeos"),
  //                           ChromeOSDataPropertyGetCallback,
  //                           v8::Local<v8::Value>(), v8::DontEnum,
  //                           v8::SideEffectType::kHasNoSideEffect)
  //     .ToChecked();

}  // namespace

void FingerprintingExtensions::Initialize() {
  // Supposed to be invoked on the main thread only
  blink::Platform* platform = Platform::Current();
  DCHECK(IsMainThread());
  DCHECK(platform);

  ExtensionsRegistry::GetInstance().RegisterBlinkExtensionInstallCallback(
      &InstallFingerprintingExtensions);

  // Do not run on snapshot. This can also run at build time lol
  if (platform->IsTakingV8ContextSnapshot()) {
    return;
  }

  mojo::Remote<fingerprinting::mojom::FingerprintManager> fingerprint_manager;
  platform->GetBrowserInterfaceBroker()->GetInterface(
      fingerprint_manager.BindNewPipeAndPassReceiver());

  base::TimeTicks start = base::TimeTicks::Now();

  // Retrieve Fingerprint from the browser process
  std::string fingerprint_str;
  bool fingerprinting_enabled = false;
  if (!fingerprint_manager->Enabled(&fingerprinting_enabled) ||
      !fingerprinting_enabled ||
      !fingerprint_manager->GetFingerprintString(&fingerprint_str) ||
      fingerprint_str.empty()) {
    LOG(ERROR) << "FingerprintingEvasionsController::Init() -> "
                  "Failed to retrieve fingerprint (PIPE)";
    return;
  }
  base::TimeTicks end = base::TimeTicks::Now();

  base::TimeDelta delta = end - start;

  base::Process procData = base::Process::Current();

  DLOG(INFO) << "Render Proc [" << procData.Pid() << "]"
             << "Retrieved fingerprint in " << delta.InMillisecondsF() << " ms";

  // get dict size
  const size_t size = fingerprint_str.length();

  // Get the renderer process ID
  DLOG(INFO) << "FingerprintExtesions::Init() -> Retrieved "
                "fingerprint in "
             << delta.InMillisecondsF()
             << " ms, size: " << std::to_string(size);

  // Set the static fingerprint
  FingerprintProvider::Set(
      new fingerprinting::Fingerprint(std::move(fingerprint_str)));

  // Create a persistent, unbound v8 JSON object that can be statically accessed
}

}  // namespace blink
