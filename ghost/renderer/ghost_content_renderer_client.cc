#include "ghost/renderer/ghost_content_renderer_client.h"
#include "base/logging.h"
#include "ghost/renderer/ghost_render_frame_observer.h"
#include "ghost/renderer/ghost_render_thread_observer.h"

GhostContentRendererClient::GhostContentRendererClient() {
  LOG(INFO) << "GhostContentRendererClient constructor";
}

// void GhostContentRendererClient::RenderThreadStarted() {
//   ChromeContentRendererClient::RenderThreadStarted();
//
//   render_thread_observer_ = std::make_unique<GhostRenderThreadObserver>();
//
//   content::RenderThread::Get()->AddObserver(render_thread_observer_.get());
//   brave_search_service_worker_holder_.SetBrowserInterfaceBrokerProxy(
//       browser_interface_broker_.get());
//
//   blink::WebScriptController::RegisterExtension(
//       brave::SafeBuiltins::CreateV8Extension());
// }

// Override the RenderFrameCreated method to just call LOG, and forward the rest
// to the superclass
void GhostContentRendererClient::RenderFrameCreated(
    content::RenderFrame* render_frame) {
  ChromeContentRendererClient::RenderFrameCreated(render_frame);

  LOG(INFO) << "GhostContentRendererClient RenderFrameCreated";

  // auto g_render_frame_observer = std::make_unique<GhostRenderFrameObserver>(
  //     render_frame, web_cache_impl_.get());
}
