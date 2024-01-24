#include "third_party/blink/renderer/extensions/ghost/fingerprinting.h"

#include "base/logging.h"
#include "base/process/current_process.h"
#include "base/process/process.h"
#include "base/time/time.h"
#include "base/values.h"
#include "components/fingerprinting/renderer/evasions/execution_context/package.h"
#include "components/fingerprinting/renderer/evasions/pack.h"
#include "fingerprinting/core/device_descriptor/fingerprint_impl.h"
#include "fingerprinting/core/evasions/hook.h"
#include "fingerprinting/core/evasions/pack.h"
#include "fingerprinting/public/cpp/manager.h"
#include "fingerprinting/public/mojom/manager.mojom.h"
#include "third_party/blink/public/common/thread_safe_browser_interface_broker_proxy.h"
#include "third_party/blink/public/mojom/browser_interface_broker.mojom-blink.h"
#include "third_party/blink/public/platform/platform.h"
#include "third_party/blink/renderer/core/execution_context/execution_context.h"
#include "third_party/blink/renderer/platform/bindings/extensions_registry.h"
#include "third_party/blink/renderer/platform/bindings/v8_set_return_value.h"
#include "third_party/blink/renderer/platform/fingerprinting/provider.h"
namespace blink {

using fingerprinting::core::evasions::EvasionsPackage;
using fingerprinting::core::evasions::EvasionsPackageExecutionContext;
using fingerprinting::core::evasions::Hook;
using fingerprinting::core::evasions::HookTargetType;
void InstallFingerprintingExtensions(ScriptState* script_state) {
  auto* execution_context = ExecutionContext::From(script_state);

  // The acutal patch logic to be executed here....

  auto global_proxy = script_state->GetContext()->Global();

  LOG(INFO) << "InstallFingerprintExtensions executing()";

  // Create an evasion pack
  ExecutionContext* exec = ExecutionContext::From(script_state);

  EvasionsPackageExecutionContext runner = EvasionsPackageExecutionContext(
      EvasionsPackage::Pack(HookTargetType::PAGE), script_state,
      FingerprintProvider::Get());

  runner.Run();

  // EvasionsPackageExecutionContext

  // global_proxy
  //     ->SetLazyDataProperty(script_state->GetContext(),
  //                           V8String(script_state->GetIsolate(), "chromeos"),
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

  // Use the fingerprint_manager to call GetFingerprintStr
  base::Value fp_value;
  if (!fingerprint_manager->GetFingerprintValue(&fp_value)) {
    LOG(ERROR) << "FingerprintingEvasionsController::Init() -> "
                  "Failed to retrieve fingerprint (PIPE)";
  }
  base::TimeTicks end = base::TimeTicks::Now();
  if (fp_value.is_none()) {
    LOG(ERROR) << "FingerprintingEvasionsController::Init() -> "
                  "Failed to retrieve fingerprint";
    // Fingerprinting is blocked, so we should not enable the API.
    return;
  }

  base::TimeDelta delta = end - start;

  base::Process procData = base::Process::Current();

  DLOG(INFO) << "Render Proc [" << procData.Pid() << "]"
             << "Retrieved fingerprint in " << delta.InMillisecondsF() << " ms";

  // get dict size
  const size_t size = fp_value.GetDict().size();

  // Get the renderer process ID
  DLOG(INFO) << "FingerprintExtesions::Init() -> Retrieved "
                "fingerprint in "
             << delta.InMillisecondsF()
             << " ms, dict-keys: " << std::to_string(size);

  // Set the static fingerprint
  FingerprintProvider::Set(new fingerprinting::Fingerprint(fp_value));
}

}  // namespace blink
