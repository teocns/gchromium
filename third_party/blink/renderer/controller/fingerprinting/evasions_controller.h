#ifndef THIRD_PARTY_BLINK_REDNERER_CONTROLLER_FINGERPRINTING_INJECTOR_H
#define THIRD_PARTY_BLINK_REDNERER_CONTROLLER_FINGERPRINTING_INJECTOR_H

#include "base/memory/scoped_refptr.h"
#include "base/task/sequenced_task_runner.h"
#include "mojo/public/cpp/bindings/remote.h"
#include "third_party/blink/public/common/tokens/tokens.h"
#include "third_party/blink/renderer/controller/controller_export.h"
#include "third_party/blink/renderer/platform/instrumentation/resource_coordinator/renderer_resource_coordinator.h"
#include "third_party/blink/renderer/platform/wtf/allocator/allocator.h"

namespace blink {

// A singleton that holds the device descriptor in Renderer Process' stack
// Monkey-patches script contexts
class CONTROLLER_EXPORT FingerprintingEvasionsController {
 // USING_FAST_MALLOC(FingerprintingEvasionsController) {
 public:
  FingerprintingEvasionsController(const FingerprintingEvasionsController&) =
      delete;
  FingerprintingEvasionsController& operator=(
      const FingerprintingEvasionsController&) = delete;
  ~FingerprintingEvasionsController() = default;
  FingerprintingEvasionsController() = default;

  static void Init();
  // These calls can arrive on any thread.
  void OnScriptStateCreated(ScriptState* script_state,
                            ExecutionContext* execution_context) {}
  void OnScriptStateDestroyed(ScriptState* script_state) {}

 private:

  // explicit FingerprintingEvasionsController(
  //     mojo::PendingRemote<
  //         performance_manager::mojom::blink::ProcessCoordinationUnit>
  //         remote);

  // Used for dispatching script state events which can arrive on any thread
  // but need to be sent outbound from the main thread.
  // void DispatchOnV8ContextCreated(
  //     performance_manager::mojom::blink::V8ContextDescriptionPtr v8_desc,
  //     performance_manager::mojom::blink::IframeAttributionDataPtr
  //         iframe_attribution_data);
  // void DispatchOnV8ContextDetached(const blink::V8ContextToken& token);
  // void DispatchOnV8ContextDestroyed(const blink::V8ContextToken& token);
  // void DispatchFireBackgroundTracingTrigger(const String& trigger_name);

  // mojo::Remote<fingerprinting::mojom::blink::ProcessCoordinationUnit>
  //     service_;

  // scoped_refptr<base::SequencedTaskRunner> service_task_runner_;
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_REDNERER_CONTROLLER_FINGERPRINTING_INJECTOR_H
