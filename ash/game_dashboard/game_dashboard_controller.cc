// Copyright 2023 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ash/game_dashboard/game_dashboard_controller.h"

#include <memory>
#include <string>

#include "ash/public/cpp/app_types_util.h"
#include "ash/public/cpp/window_properties.h"
#include "chromeos/ui/base/window_properties.h"
#include "extensions/common/constants.h"
#include "ui/aura/client/window_types.h"
#include "ui/aura/window.h"

namespace ash {

namespace {
// The singleton instance owned by `Shell`.
GameDashboardController* g_instance = nullptr;
}  // namespace

// static
GameDashboardController* GameDashboardController::Get() {
  return g_instance;
}

GameDashboardController::GameDashboardController(
    std::unique_ptr<GameDashboardDelegate> delegate)
    : delegate_(std::move(delegate)) {
  DCHECK_EQ(g_instance, nullptr);
  g_instance = this;
  CHECK(aura::Env::HasInstance());
  env_observation_.Observe(aura::Env::GetInstance());
}

GameDashboardController::~GameDashboardController() {
  DCHECK_EQ(g_instance, this);
  g_instance = nullptr;
}

void GameDashboardController::OnWindowInitialized(aura::Window* new_window) {
  auto* top_level_window = new_window->GetToplevelWindow();
  if (!top_level_window ||
      top_level_window->GetType() != aura::client::WINDOW_TYPE_NORMAL) {
    // Ignore non-NORMAL window types.
    return;
  }
  RefreshWindowTracking(new_window);
}

void GameDashboardController::OnWindowPropertyChanged(aura::Window* window,
                                                      const void* key,
                                                      intptr_t old) {
  if (key == kAppIDKey) {
    RefreshWindowTracking(window);
  }
}

void GameDashboardController::OnWindowDestroying(aura::Window* window) {
  window_observations_.RemoveObservation(window);
}

GameDashboardController::WindowGameState
GameDashboardController::GetWindowGameState(aura::Window* window) const {
  const auto* app_id = window->GetProperty(kAppIDKey);
  if (!app_id) {
    return WindowGameState::kNotYetKnown;
  }
  const bool is_game = (IsArcWindow(window) && delegate_->IsGame(*app_id)) ||
                       (*app_id == extension_misc::kGeForceNowAppId);
  return is_game ? WindowGameState::kGame : WindowGameState::kNotGame;
}

void GameDashboardController::RefreshWindowTracking(aura::Window* window) {
  const bool is_observing = window_observations_.IsObservingSource(window);
  const auto state = GetWindowGameState(window);
  const bool should_observe = state != WindowGameState::kNotGame;

  if (state != WindowGameState::kNotYetKnown) {
    window->SetProperty(chromeos::kIsGameKey, state == WindowGameState::kGame);
  }

  if (is_observing == should_observe) {
    return;
  }

  if (should_observe) {
    window_observations_.AddObservation(window);
  } else {
    window_observations_.RemoveObservation(window);
  }
}

}  // namespace ash
