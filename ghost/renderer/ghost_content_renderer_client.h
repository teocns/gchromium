
/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef GHOST_CONTENT_RENDERER_CLIENT_H_
#define GHOST_CONTENT_RENDERER_CLIENT_H_

#include <memory>

// #include
// "brave/components/brave_search/renderer/brave_search_service_worker_holder.h"
#include "chrome/renderer/chrome_content_renderer_client.h"
#include "ghost/renderer/ghost_render_frame_observer.h"
#include "ghost/renderer/ghost_render_thread_observer.h"
// #include "v8/include/v8.h"

class BraveRenderThreadObserver;
class GURL;

namespace blink {
class WebServiceWorkerContextProxy;
}

// Responsible for initializing the renderer process, setting up observers, etc.
class GhostContentRendererClient : public ChromeContentRendererClient {
 public:
 using ChromeContentRendererClient::ChromeContentRendererClient;
  // GhostContentRendererClient();

  // void RenderFrameCreated(content::RenderFrame* render_frame) override;
  //
  // void RenderThreadStarted() override;

 private:
  std::unique_ptr<GhostRenderThreadObserver> render_thread_observer_;
};

#endif  // GHOST_CONTENT_RENDERER_CLIENT_H_
