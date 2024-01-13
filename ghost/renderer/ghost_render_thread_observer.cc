#include "ghost/renderer/ghost_render_thread_observer.h"
GhostRenderThreadObserver::~GhostRenderThreadObserver() = default;
GhostRenderThreadObserver::GhostRenderThreadObserver() {
  LOG(INFO) << "GhostRenderThreadObserver constructor";
}
