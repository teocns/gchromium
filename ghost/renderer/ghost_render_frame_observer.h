
// Copyright 2019 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GHOST_RENDERER_GHOST_RENDER_FRAME_OBSERVER_H
#define GHOST_RENDERER_GHOST_RENDER_FRAME_OBSERVER_H

// #include <memory>
//
// #include "base/containers/circular_deque.h"
// #include "base/containers/contains.h"
// #include "base/functional/bind.h"
// #include "base/memory/weak_ptr.h"
// #include "base/no_destructor.h"
#include "content/public/renderer/render_frame.h"
#include "content/public/renderer/render_frame_observer.h"
// #include "v8/include/v8-context.h"
// #include "v8/include/v8-forward.h"
// #include "v8/include/v8-function.h"
// #include "v8/include/v8-microtask-queue.h"
// #include "v8/include/v8-persistent-handle.h"
// #include "v8/include/v8-primitive.h"

namespace content {
class RenderFrame;
}

class GhostRenderFrameObserver {
 public:
  GhostRenderFrameObserver(content::RenderFrame* render_frame
                           /* web_cache::WebCacheImpl* web_cache_impl */);
  // void RegisterMojoInterfaces(
  //     blink::AssociatedInterfaceRegistry* associated_interfaces) override {}
  // void UnregisterMojoInterfaces(
  //     blink::AssociatedInterfaceRegistry* associated_interfaces) override {}

  // Allows filtering of control messages.
  // bool OnControlMessageReceived(const IPC::Message& message);

  // Called when the renderer cache of the plugin list has changed.
  // virtual void PluginListChanged() {}
};

#endif  // GHOST_RENDERER_GHOST_RENDER_FRAME_OBSERVER_H
