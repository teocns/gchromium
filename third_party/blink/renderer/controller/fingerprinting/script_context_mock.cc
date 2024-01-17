

#include "third_party/blink/renderer/controller/fingerprinting/script_context_mock.h"

void blink::FingerprintScriptContextMock::MaybeInitialize() {
  // if (!RuntimeEnabledFeatures::PerformanceManagerInstrumentationEnabled())
  //     return;

  blink::Platform* platform = Platform::Current();
  DCHECK(IsMainThread());
  DCHECK(platform);

  // TODO: define ProcessCoordinationUnit 
  mojo::PendingRemote<ProcessCoordinationUnit> remote;
  platform->GetBrowserInterfaceBroker()->GetInterface(
      remote.InitWithNewPipeAndPassReceiver());
  RendererResourceCoordinator::Set(
      new RendererResourceCoordinatorImpl(std::move(remote)));
}
