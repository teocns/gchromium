#ifndef GHOST_RENDERER_GHOST_RENDER_THREAD_OBSERVER_H_
#define GHOST_RENDERER_GHOST_RENDER_THREAD_OBSERVER_H_

#include "chrome/common/renderer_configuration.mojom.h"
#include "chrome/renderer/chrome_render_thread_observer.h"

// This class filters the incoming control messages (i.e. ones not destined for
// a RenderView) for Chrome specific messages that the content layer doesn't
// happen.  If a few messages are related, they should probably have their own
// observer.
class GhostRenderThreadObserver : public ChromeRenderThreadObserver {};

#endif  // GHOST_RENDERER_GHOST_RENDER_THREAD_OBSERVER_H_
