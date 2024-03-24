
// Copyright 2019 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// #include "gin/handle.h"
// #include "third_party/blink/public/web/blink.h"
// #include "third_party/blink/public/web/web_local_frame.h"

#include "ghost/renderer/ghost_render_frame_observer.h"
#include "base/logging.h"

GhostRenderFrameObserver::GhostRenderFrameObserver(
    content::RenderFrame* render_frame) {
  LOG(INFO) << "GhostRenderFrameObserver constructor";
}
// namespace fingerprinting {

// void GhostRenderFrameObserver::DidCreateScriptContext(
//     v8::Local<v8::Context> context,
//     int32_t world_id) {}
//
// void GhostRenderFrameObserver::Install(content::RenderFrame* render_frame,
//                                      blink::WebLocalFrame* frame) {
//   v8::Isolate* isolate = blink::MainThreadIsolate();
//   v8::HandleScope handle_scope(isolate);
//   v8::Local<v8::Context> context = frame->MainWorldScriptContext();
//   if (context.IsEmpty()) {
//     return;
//   }
//
//   v8::Context::Scope context_scope(context);
//   //
//   // gin::Handle<DomAutomationController> controller =
//   //     gin::CreateHandle(isolate, new
//   DomAutomationController(render_frame));
//   // if (controller.IsEmpty()) {
//   //   return;
//   // }
//
//   // Here set the functions
//   v8::Local<v8::Object> global = context->Global();
//   global
//       ->Set(context, gin::StringToV8(isolate, "domAutomationController"),
//             controller.ToV8())
//       .Check();
// }
// }  // namespace fingerprinting
