#ifndef GHOST_RENDERER_GHOST_RENDER_THREAD_OBSERVER_H_
#define GHOST_RENDERER_GHOST_RENDER_THREAD_OBSERVER_H_

#include "chrome/common/renderer_configuration.mojom.h"
#include "chrome/renderer/chrome_render_thread_observer.h"
#include "content/public/renderer/render_thread_observer.h"
#include "mojo/public/cpp/bindings/associated_receiver_set.h"
#include "mojo/public/cpp/bindings/pending_associated_receiver.h"
#include "mojo/public/cpp/bindings/pending_receiver.h"
#include "mojo/public/cpp/bindings/receiver.h"

// #include "base/logging.h"

// This class filters the incoming control messages (i.e. ones not destined for
// a RenderView) for Chrome specific messages that the content layer doesn't
// happen.  If a few messages are related, they should probably have their own
// observer.
// class GhostRenderThreadObserver : public content::RenderThreadObserver,
//                                   public chrome::mojom::RendererConfiguration
//                                   {

class GhostRenderThreadObserver : public content::RenderThreadObserver {
 public:
  GhostRenderThreadObserver(const GhostRenderThreadObserver&) = delete;
  GhostRenderThreadObserver& operator=(const GhostRenderThreadObserver&) =
      delete;
  ~GhostRenderThreadObserver() override;

  GhostRenderThreadObserver();
  // void RegisterMojoInterfaces(
  //     blink::AssociatedInterfaceRegistry* associated_interfaces) override;
  // void UnregisterMojoInterfaces(
  //     blink::AssociatedInterfaceRegistry* associated_interfaces) override;
  //
  // // brave::mojom::BraveRendererConfiguration:
  // void SetInitialConfiguration(bool is_tor_process) override;
  // void SetConfiguration(brave::mojom::DynamicParamsPtr params);
  //
  // void OnRendererConfigurationAssociatedRequest(
  //     mojo::PendingAssociatedReceiver<brave::mojom::BraveRendererConfiguration>
  //         receiver);
  //
  // mojo::AssociatedReceiverSet<brave::mojom::BraveRendererConfiguration>
  //     renderer_configuration_receivers_;
};

#endif  // GHOST_RENDERER_GHOST_RENDER_THREAD_OBSERVER_H_
