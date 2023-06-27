// Copyright 2023 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/browser/ash/arc/input_overlay/ui/edit_label.h"

#include <memory>
#include <vector>

#include "ash/constants/ash_features.h"
#include "chrome/browser/ash/arc/input_overlay/actions/action.h"
#include "chrome/browser/ash/arc/input_overlay/constants.h"
#include "chrome/browser/ash/arc/input_overlay/db/proto/app_data.pb.h"
#include "chrome/browser/ash/arc/input_overlay/display_overlay_controller.h"
#include "chrome/browser/ash/arc/input_overlay/test/view_test_base.h"
#include "chrome/browser/ash/arc/input_overlay/ui/action_view.h"
#include "chrome/browser/ash/arc/input_overlay/ui/action_view_list_item.h"
#include "chrome/browser/ash/arc/input_overlay/ui/button_options_menu.h"
#include "chrome/browser/ash/arc/input_overlay/ui/edit_label.h"
#include "chrome/browser/ash/arc/input_overlay/ui/edit_labels.h"
#include "chrome/browser/ash/arc/input_overlay/ui/editing_list.h"
#include "chrome/browser/ash/arc/input_overlay/ui/name_tag.h"
#include "ui/events/keycodes/keyboard_codes_posix.h"

namespace arc::input_overlay {

namespace {

class TestButtonOptionsMenu : public ButtonOptionsMenu {
 public:
  TestButtonOptionsMenu(DisplayOverlayController* controller, Action* action)
      : ButtonOptionsMenu(controller, action) {}
  ~TestButtonOptionsMenu() override = default;

 private:
  // ButtonOptionsMenu:
  void CalculatePosition() override {}
};

}  // namespace

class EditLabelTest : public ViewTestBase {
 public:
  EditLabelTest() = default;

  EditLabel* GetEditLabel(const ActionViewListItem* list_item,
                          size_t index) const {
    auto& labels = list_item->labels_view_->labels_;
    DCHECK_LT(index, labels.size());
    return labels[index];
  }

  EditLabel* GetEditLabel(const ButtonOptionsMenu* menu, size_t index) const {
    auto& labels = menu->labels_view_->labels_;
    DCHECK_LT(index, labels.size());
    return labels[index];
  }

  ActionLabel* GetLabel(const ActionView* action_view, size_t index) const {
    auto& labels = action_view->labels();
    DCHECK_LT(index, labels.size());
    return labels[index];
  }

  void TapKeyboardKeyOnEditLabel(EditLabel* label, ui::KeyboardCode code) {
    label->OnKeyPressed(ui::KeyEvent(ui::ET_KEY_PRESSED, code, ui::EF_NONE));
    label->OnKeyReleased(ui::KeyEvent(ui::ET_KEY_RELEASED, code, ui::EF_NONE));
  }

  void FocusOnLabel(EditLabel* label) {
    DCHECK(label);
    label->OnFocus();
  }

  void BlurOnLabel(EditLabel* label) {
    DCHECK(label);
    label->OnBlur();
  }

  bool IsInErrorState(ButtonOptionsMenu* menu) {
    DCHECK(menu);
    return IsNameTagInErrorState(menu->labels_view_);
  }

  bool IsInErrorState(ActionViewListItem* list_item) {
    DCHECK(list_item);
    return IsNameTagInErrorState(list_item->labels_view_);
  }

  void CheckAction(ActionType action_type,
                   const std::vector<ui::DomCode>& expected_code,
                   const std::vector<std::u16string>& expected_text) {
    switch (action_type) {
      case ActionType::TAP:
        if (expected_code.empty()) {
          EXPECT_TRUE(tap_action_->current_input()->keys().empty());
        } else {
          EXPECT_EQ(expected_code[0], tap_action_->current_input()->keys()[0]);
        }
        EXPECT_EQ(expected_text[0],
                  GetEditLabel(tap_action_list_item_, /*index=*/0)->GetText());
        EXPECT_EQ(expected_text[0],
                  GetEditLabel(tap_action_menu_.get(), /*index=*/0)->GetText());
        EXPECT_EQ(expected_text[0],
                  GetLabel(tap_action_view_, /*index=*/0)->GetText());
        break;
      case ActionType::MOVE:
        for (size_t i = 0; i < kActionMoveKeysSize; i++) {
          EXPECT_EQ(expected_code[i], move_action_->current_input()->keys()[i]);
          EXPECT_EQ(
              expected_text[i],
              GetEditLabel(move_action_list_item_, /*index=*/i)->GetText());
          EXPECT_EQ(
              expected_text[i],
              GetEditLabel(move_action_menu_.get(), /*index=*/i)->GetText());
          EXPECT_EQ(expected_text[i],
                    GetLabel(move_action_view_, /*index=*/i)->GetText());
        }
        break;
      default:
        NOTREACHED();
    }
  }

  void CheckErrorState(ActionType action_type,
                       bool menu_has_error,
                       bool list_item_has_error) {
    switch (action_type) {
      case ActionType::TAP:
        EXPECT_EQ(menu_has_error, IsInErrorState(tap_action_menu_.get()));
        EXPECT_EQ(list_item_has_error,
                  IsInErrorState(tap_action_list_item_.get()));
        break;
      case ActionType::MOVE:
        EXPECT_EQ(menu_has_error, IsInErrorState(move_action_menu_.get()));
        EXPECT_EQ(list_item_has_error,
                  IsInErrorState(move_action_list_item_.get()));
        break;
      default:
        NOTREACHED();
    }
  }

  std::unique_ptr<EditingList> editing_list_;
  std::unique_ptr<TestButtonOptionsMenu> tap_action_menu_;
  std::unique_ptr<TestButtonOptionsMenu> move_action_menu_;
  raw_ptr<ActionViewListItem, DanglingUntriaged> tap_action_list_item_;
  raw_ptr<ActionViewListItem, DanglingUntriaged> move_action_list_item_;

 private:
  // Checks if the name tag attached to |edit_labels| is in error state.
  bool IsNameTagInErrorState(EditLabels* edit_labels) {
    DCHECK(edit_labels);
    NameTag* name_tag = edit_labels->name_tag_;
    DCHECK(name_tag);
    views::ImageView* error_icon = name_tag->error_icon_;
    DCHECK(error_icon);
    return error_icon->GetVisible();
  }

  void SetUp() override {
    ViewTestBase::SetUp();
    InitWithFeature(ash::features::kArcInputOverlayBeta);
    SetDisplayMode(DisplayMode::kEdit);

    editing_list_ =
        std::make_unique<EditingList>(display_overlay_controller_.get());
    editing_list_->Init();
    DCHECK(editing_list_->scroll_content_);
    const auto& items = editing_list_->scroll_content_->children();
    DCHECK_EQ(items.size(), 2u);
    tap_action_list_item_ = static_cast<ActionViewListItem*>(items[0]);
    move_action_list_item_ = static_cast<ActionViewListItem*>(items[1]);
    DCHECK(tap_action_list_item_);
    DCHECK(move_action_list_item_);

    tap_action_menu_ = std::make_unique<TestButtonOptionsMenu>(
        display_overlay_controller_.get(), tap_action_);
    tap_action_menu_->Init();
    move_action_menu_ = std::make_unique<TestButtonOptionsMenu>(
        display_overlay_controller_.get(), move_action_);
    move_action_menu_->Init();
  }

  void TearDown() override {
    move_action_menu_.reset();
    tap_action_menu_.reset();
    editing_list_.reset();
    ViewTestBase::TearDown();
  }
};

TEST_F(EditLabelTest, TestEditingListLabelEditing) {
  // Modify the label for ActionTap and noting is conflicted.
  // ActionTap: ␣ -> m.
  CheckAction(ActionType::TAP, {ui::DomCode::SPACE}, {u"␣"});
  CheckErrorState(ActionType::TAP, /*menu_has_error=*/false,
                  /*list_item_has_error=*/false);
  TapKeyboardKeyOnEditLabel(GetEditLabel(tap_action_list_item_, /*index=*/0),
                            ui::VKEY_M);
  CheckAction(ActionType::TAP, {ui::DomCode::US_M}, {u"m"});
  CheckErrorState(ActionType::TAP, /*menu_has_error=*/false,
                  /*list_item_has_error=*/false);

  // Modify the label for ActionMove and nothing is conflicted.
  // ActionMove: wasd -> lasd.
  CheckAction(ActionType::MOVE,
              {ui::DomCode::US_W, ui::DomCode::US_A, ui::DomCode::US_S,
               ui::DomCode::US_D},
              {u"w", u"a", u"s", u"d"});
  CheckErrorState(ActionType::MOVE, /*menu_has_error=*/false,
                  /*list_item_has_error=*/false);

  TapKeyboardKeyOnEditLabel(GetEditLabel(move_action_list_item_, /*index=*/0),
                            ui::VKEY_L);
  CheckAction(ActionType::MOVE,
              {ui::DomCode::US_L, ui::DomCode::US_A, ui::DomCode::US_S,
               ui::DomCode::US_D},
              {u"l", u"a", u"s", u"d"});
  CheckErrorState(ActionType::MOVE, /*menu_has_error=*/false,
                  /*list_item_has_error=*/false);

  // Modify the label for ActionMove and it is conflicted inside.
  // ActionMove: lasd -> ?ald.
  TapKeyboardKeyOnEditLabel(GetEditLabel(move_action_list_item_, /*index=*/2),
                            ui::VKEY_L);
  CheckAction(ActionType::MOVE,
              {ui::DomCode::NONE, ui::DomCode::US_A, ui::DomCode::US_L,
               ui::DomCode::US_D},
              {u"?", u"a", u"l", u"d"});
  CheckErrorState(ActionType::MOVE, /*menu_has_error=*/true,
                  /*list_item_has_error=*/true);

  // Modify the label for ActionMove and it is conflicted outside.
  // ActionTap: m -> ?
  // ActionMove: ?ald -> mald.
  TapKeyboardKeyOnEditLabel(GetEditLabel(move_action_list_item_, /*index=*/0),
                            ui::VKEY_M);
  CheckAction(ActionType::TAP, {}, {u"?"});
  CheckErrorState(ActionType::TAP, /*menu_has_error=*/true,
                  /*list_item_has_error=*/true);
  CheckAction(ActionType::MOVE,
              {ui::DomCode::US_M, ui::DomCode::US_A, ui::DomCode::US_L,
               ui::DomCode::US_D},
              {u"m", u"a", u"l", u"d"});
  CheckErrorState(ActionType::MOVE, /*menu_has_error=*/false,
                  /*list_item_has_error=*/false);

  // Modify the label for ActionTap and it is conflicted outside.
  // ActionTap: ? -> d.
  // ActionMove: mald -> mal?.
  TapKeyboardKeyOnEditLabel(GetEditLabel(tap_action_list_item_, /*index=*/0),
                            ui::VKEY_D);
  CheckAction(ActionType::TAP, {ui::DomCode::US_D}, {u"d"});
  CheckErrorState(ActionType::TAP, /*menu_has_error=*/false,
                  /*list_item_has_error=*/false);
  CheckAction(ActionType::MOVE,
              {ui::DomCode::US_M, ui::DomCode::US_A, ui::DomCode::US_L,
               ui::DomCode::NONE},
              {u"m", u"a", u"l", u"?"});
  CheckErrorState(ActionType::MOVE, /*menu_has_error=*/true,
                  /*list_item_has_error=*/true);
}

TEST_F(EditLabelTest, TestEditingListLabelReservedKey) {
  // Press a reserved key on Action tap with no error state and then it shows
  // error state.
  FocusOnLabel(GetEditLabel(tap_action_list_item_, /*index=*/0));
  TapKeyboardKeyOnEditLabel(GetEditLabel(tap_action_list_item_, /*index=*/0),
                            ui::VKEY_ESCAPE);
  // Label is not changed.
  CheckAction(ActionType::TAP, {ui::DomCode::SPACE}, {u"␣"});
  // Error state shows temporarily on list item view.
  CheckErrorState(ActionType::TAP, /*menu_has_error=*/false,
                  /*list_item_has_error=*/true);
  // Error state shows up temporarily and disappears after leaving focus.
  BlurOnLabel(GetEditLabel(tap_action_list_item_, /*index=*/0));
  CheckErrorState(ActionType::TAP, /*menu_has_error=*/false,
                  /*list_item_has_error=*/false);

  // Press a reserved key on Action move which is already in error state.
  // ActionMove: wasd -> wal?.
  FocusOnLabel(GetEditLabel(tap_action_list_item_, /*index=*/0));
  TapKeyboardKeyOnEditLabel(GetEditLabel(tap_action_list_item_, /*index=*/0),
                            ui::VKEY_D);
  CheckAction(ActionType::MOVE,
              {ui::DomCode::US_W, ui::DomCode::US_A, ui::DomCode::US_S,
               ui::DomCode::NONE},
              {u"w", u"a", u"s", u"?"});
  CheckErrorState(ActionType::MOVE, /*menu_has_error=*/true,
                  /*list_item_has_error=*/true);
  FocusOnLabel(GetEditLabel(move_action_list_item_, /*index=*/0));
  // Press a reserved key on Action move and error state still shows up.
  TapKeyboardKeyOnEditLabel(GetEditLabel(move_action_list_item_, /*index=*/0),
                            ui::VKEY_ESCAPE);
  CheckErrorState(ActionType::MOVE, /*menu_has_error=*/true,
                  /*list_item_has_error=*/true);
  BlurOnLabel(GetEditLabel(move_action_list_item_, /*index=*/0));
  // Error state still shows up after leaving focus.
  CheckErrorState(ActionType::MOVE, /*menu_has_error=*/true,
                  /*list_item_has_error=*/true);
}

}  // namespace arc::input_overlay
