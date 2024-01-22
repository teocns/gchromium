#include "third_party/blink/renderer/controller/fingerprinting/evasions_controller.h"
#include "base/logging.h"
#include "base/process/current_process.h"
#include "base/process/process.h"
#include "base/time/time.h"
#include "base/values.h"
#include "components/fingerprinting/common/manager.h"
#include "components/fingerprinting/common/mojom/manager.mojom-blink.h"
#include "third_party/blink/public/common/thread_safe_browser_interface_broker_proxy.h"
#include "third_party/blink/public/mojom/browser_interface_broker.mojom-blink.h"
#include "third_party/blink/public/platform/platform.h"
// #include
// "third_party/blink/renderer/controller/fingerprinting/script_context_mock.h"

namespace blink {

// Fetch the device descriptor from the browser process via IPC
void FingerprintingEvasionsController::Init() {
  // Supposed to be invoked on the main thread only
  blink::Platform* platform = Platform::Current();
  DCHECK(IsMainThread());
  DCHECK(platform);

  // Do not run on snapshot
  if (platform->IsTakingV8ContextSnapshot()) {
    return;
  }

  mojo::Remote<fingerprinting::mojom::FingerprintManager> fingerprint_manager;
  platform->GetBrowserInterfaceBroker()->GetInterface(
      fingerprint_manager.BindNewPipeAndPassReceiver());

  base::TimeTicks start = base::TimeTicks::Now();

  // Use the fingerprint_manager to call GetFingerprintStr
  std::string fingerprint_str;
  if (!fingerprint_manager->GetFingerprintString(&fingerprint_str)) {
    LOG(ERROR) << "FingerprintingEvasionsController::Init() -> "
                  "Failed to retrieve fingerprint (PIPE)";
  }
  base::TimeTicks end = base::TimeTicks::Now();
  if (fingerprint_str.empty()) {
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
  const size_t size = fingerprint_str.length();

  // Get the renderer process ID
  DLOG(INFO) << "FingerprintingEvasionsController::Init() -> Retrieved "
                "fingerprint in "
             << delta.InMillisecondsF() << " ms, dict-keys: " << size;
  absl::optional<fingerprinting::Fingerprint> fp =
      fingerprinting::Fingerprint::From(fingerprint_str);

  if (!fp.has_value()) {
    LOG(ERROR) << "FingerprintingEvasionsController::Init() -> "
                  "Failede to parse fingerprint From()";
    // Fingerprinting is blocked, so we should not enable the API.
    return;
  }
  fingerprinting::FingerprintManager::GetInstance()->SetFingerprint(

      std::make_unique<Fingerprint>(std::move(fp.value())));
}
}  // namespace blink
