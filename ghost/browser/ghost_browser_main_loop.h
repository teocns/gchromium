/* Copyright 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef GHOST_BROWSER_GHOST_BROWSER_MAIN_LOOP_H_
#define GHOST_BROWSER_GHOST_BROWSER_MAIN_LOOP_H_

#include "content/browser/browser_main_loop.h"

namespace ghost {

class GhostBrowserMainLoop : public content::BrowserMainLoop {
 public:
  using BrowserMainLoop::BrowserMainLoop;

  GhostBrowserMainLoop(const GhostBrowserMainLoop&) = delete;
  GhostBrowserMainLoop& operator=(const GhostBrowserMainLoop&) = delete;
  ~GhostBrowserMainLoop() override = default;

  void CreateStartupTasks() override;

};

}  // namespace brave

#endif  // GHOST_BROWSER_GHOST_BROWSER_MAIN_LOOP_H_
