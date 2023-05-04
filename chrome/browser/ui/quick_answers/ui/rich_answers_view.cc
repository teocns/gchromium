// Copyright 2023 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/browser/ui/quick_answers/ui/rich_answers_view.h"

#include "base/functional/bind.h"
#include "base/memory/raw_ptr.h"
#include "chrome/browser/ui/color/chrome_color_id.h"
#include "chrome/browser/ui/quick_answers/quick_answers_ui_controller.h"
#include "chrome/browser/ui/quick_answers/ui/quick_answers_util.h"
#include "chrome/browser/ui/quick_answers/ui/quick_answers_view.h"
#include "chrome/browser/ui/quick_answers/ui/rich_answers_definition_view.h"
#include "chrome/browser/ui/quick_answers/ui/rich_answers_pre_target_handler.h"
#include "chrome/browser/ui/quick_answers/ui/rich_answers_translation_view.h"
#include "chrome/browser/ui/quick_answers/ui/rich_answers_unit_conversion_view.h"
#include "chromeos/components/quick_answers/quick_answers_model.h"
#include "chromeos/strings/grit/chromeos_strings.h"
#include "components/vector_icons/vector_icons.h"
#include "ui/aura/window.h"
#include "ui/base/l10n/l10n_util.h"
#include "ui/base/themed_vector_icon.h"
#include "ui/chromeos/styles/cros_tokens_color_mappings.h"
#include "ui/color/color_id.h"
#include "ui/color/color_provider.h"
#include "ui/display/screen.h"
#include "ui/gfx/paint_vector_icon.h"
#include "ui/gfx/vector_icon_types.h"
#include "ui/views/accessibility/view_accessibility.h"
#include "ui/views/background.h"
#include "ui/views/border.h"
#include "ui/views/controls/button/button.h"
#include "ui/views/controls/button/button_controller.h"
#include "ui/views/controls/button/image_button.h"
#include "ui/views/controls/image_view.h"
#include "ui/views/controls/label.h"
#include "ui/views/layout/fill_layout.h"
#include "ui/views/layout/flex_layout.h"
#include "ui/views/layout/flex_layout_view.h"
#include "ui/views/widget/widget.h"
#include "ui/wm/core/coordinate_conversion.h"

namespace {

using quick_answers::QuickAnswer;
using quick_answers::QuickAnswerResultText;
using quick_answers::ResultType;

constexpr auto kMainViewInsets = gfx::Insets::TLBR(20, 20, 0, 20);

// Buttons view.
constexpr auto kSettingsButtonInsets = gfx::Insets::TLBR(20, 8, 0, 20);
constexpr int kSettingsButtonSizeDip = 20;

// Border corner radius.
constexpr int kBorderCornerRadius = 12;

// Result type icons.
constexpr int kResultTypeIconContainerRadius = 24;
constexpr int kResultTypeIconSizeDip = 16;
constexpr auto kResultTypeIconContainerInsets = gfx::Insets::TLBR(4, 4, 4, 4);

}  // namespace

namespace quick_answers {

class RichAnswersTextLabel : public views::Label {
 public:
  explicit RichAnswersTextLabel(
      const std::u16string& text,
      ui::ColorId color_id = ui::kColorLabelForeground)
      : Label(text) {
    SetHorizontalAlignment(gfx::HorizontalAlignment::ALIGN_LEFT);
    SetEnabledColorId(color_id);
  }

  RichAnswersTextLabel(const RichAnswersTextLabel&) = delete;
  RichAnswersTextLabel& operator=(const RichAnswersTextLabel&) = delete;

  ~RichAnswersTextLabel() override = default;
};

// RichAnswersView -----------------------------------------------------------

RichAnswersView::RichAnswersView(
    const gfx::Rect& anchor_view_bounds,
    base::WeakPtr<QuickAnswersUiController> controller,
    const quick_answers::QuickAnswer& result)
    : anchor_view_bounds_(anchor_view_bounds),
      controller_(std::move(controller)),
      result_(result),
      rich_answers_view_handler_(
          std::make_unique<quick_answers::RichAnswersPreTargetHandler>(this)),
      focus_search_(std::make_unique<QuickAnswersFocusSearch>(
          this,
          base::BindRepeating(&RichAnswersView::GetFocusableViews,
                              base::Unretained(this)))) {
  InitLayout();
  InitWidget();

  // Focus.
  SetFocusBehavior(views::View::FocusBehavior::ALWAYS);
  set_suppress_default_focus_handling();
}

RichAnswersView::~RichAnswersView() = default;

const char* RichAnswersView::GetClassName() const {
  return "RichAnswersView";
}

void RichAnswersView::OnFocus() {
  View* wants_focus = focus_search_->FindNextFocusableView(
      nullptr, views::FocusSearch::SearchDirection::kForwards,
      views::FocusSearch::TraversalDirection::kDown,
      views::FocusSearch::StartingViewPolicy::kCheckStartingView,
      views::FocusSearch::AnchoredDialogPolicy::kSkipAnchoredDialog, nullptr,
      nullptr);
  if (wants_focus != this) {
    wants_focus->RequestFocus();
  } else {
    NotifyAccessibilityEvent(ax::mojom::Event::kFocus, true);
  }
}

void RichAnswersView::OnThemeChanged() {
  views::View::OnThemeChanged();
  SetBorder(views::CreateRoundedRectBorder(
      /*thickness=*/2, kBorderCornerRadius,
      GetColorProvider()->GetColor(ui::kColorPrimaryBackground)));
  SetBackground(views::CreateRoundedRectBackground(
      GetColorProvider()->GetColor(ui::kColorPrimaryBackground),
      kBorderCornerRadius, /*for_border_thickness=*/2));
}

views::FocusTraversable* RichAnswersView::GetPaneFocusTraversable() {
  return focus_search_.get();
}

void RichAnswersView::GetAccessibleNodeData(ui::AXNodeData* node_data) {
  node_data->role = ax::mojom::Role::kDialog;

  node_data->SetName(
      l10n_util::GetStringUTF8(IDS_RICH_ANSWERS_VIEW_A11Y_NAME_TEXT));
}

ui::ImageModel RichAnswersView::GetIconImageModelForTesting() {
  return vector_icon_ ? vector_icon_->GetImageModel() : ui::ImageModel();
}

void RichAnswersView::InitLayout() {
  SetLayoutManager(std::make_unique<views::FillLayout>());

  // Base view Layout.
  base_view_ = AddChildView(std::make_unique<View>());
  auto* base_layout =
      base_view_->SetLayoutManager(std::make_unique<views::FlexLayout>());
  base_layout->SetOrientation(views::LayoutOrientation::kVertical)
      .SetCrossAxisAlignment(views::LayoutAlignment::kStretch);

  main_view_ = base_view_->AddChildView(
      views::Builder<views::FlexLayoutView>()
          .SetOrientation(views::LayoutOrientation::kHorizontal)
          .SetCrossAxisAlignment(views::LayoutAlignment::kStart)
          .SetInteriorMargin(kMainViewInsets)
          .Build());

  switch (result_.result_type) {
    case quick_answers::ResultType::kDefinitionResult: {
      content_view_ = main_view_->AddChildView(
          std::make_unique<RichAnswersDefinitionView>(result_));
      break;
    }
    case quick_answers::ResultType::kTranslationResult: {
      content_view_ = main_view_->AddChildView(
          std::make_unique<RichAnswersTranslationView>(result_));
      break;
    }
    case quick_answers::ResultType::kUnitConversionResult: {
      content_view_ = main_view_->AddChildView(
          std::make_unique<RichAnswersUnitConversionView>(result_));
      break;
    }
    default: {
      break;
    }
  }

  // Add icon that corresponds to the quick answer result type.
  AddResultTypeIcon();

  // Add util buttons in the top-right corner.
  AddFrameButtons();
}

void RichAnswersView::InitWidget() {
  views::Widget::InitParams params;
  params.activatable = views::Widget::InitParams::Activatable::kNo;
  params.shadow_elevation = 2;
  params.shadow_type = views::Widget::InitParams::ShadowType::kDrop;
  params.type = views::Widget::InitParams::TYPE_POPUP;
  params.z_order = ui::ZOrderLevel::kFloatingUIElement;
  params.corner_radius = kBorderCornerRadius;
  params.name = kWidgetName;

  views::Widget* widget = new views::Widget();
  widget->Init(std::move(params));
  widget->SetContentsView(this);
  UpdateBounds();
}

void RichAnswersView::AddResultTypeIcon() {
  // Add the icon representing the quick answers result type as well as
  // a circle background behind the icon.
  raw_ptr<views::FlexLayoutView> vector_icon_container =
      main_view_->AddChildView(std::make_unique<views::FlexLayoutView>());
  vector_icon_container->SetBackground(views::CreateThemedRoundedRectBackground(
      cros_tokens::kCrosSysPrimary, kResultTypeIconContainerRadius));
  vector_icon_container->SetBorder(
      views::CreateEmptyBorder(kResultTypeIconContainerInsets));

  vector_icon_ =
      vector_icon_container->AddChildView(std::make_unique<views::ImageView>());
  vector_icon_->SetImage(
      ui::ImageModel::FromVectorIcon(GetResultTypeIcon(result_.result_type),
                                     cros_tokens::kCrosSysSystemBaseElevated,
                                     /*icon_size=*/kResultTypeIconSizeDip));
}

void RichAnswersView::AddFrameButtons() {
  auto* buttons_view =
      AddChildView(views::Builder<views::FlexLayoutView>()
                       .SetOrientation(views::LayoutOrientation::kHorizontal)
                       .SetMainAxisAlignment(views::LayoutAlignment::kEnd)
                       .SetCrossAxisAlignment(views::LayoutAlignment::kStart)
                       .SetInteriorMargin(kSettingsButtonInsets)
                       .Build());

  settings_button_ = buttons_view->AddChildView(
      std::make_unique<views::ImageButton>(base::BindRepeating(
          &QuickAnswersUiController::OnSettingsButtonPressed, controller_)));
  settings_button_->SetImageModel(
      views::Button::ButtonState::STATE_NORMAL,
      ui::ImageModel::FromVectorIcon(vector_icons::kSettingsOutlineIcon,
                                     cros_tokens::kColorPrimary,
                                     /*icon_size=*/kSettingsButtonSizeDip));
  settings_button_->SetTooltipText(l10n_util::GetStringUTF16(
      IDS_QUICK_ANSWERS_SETTINGS_BUTTON_TOOLTIP_TEXT));
}

void RichAnswersView::UpdateBounds() {
  auto display_bounds = display::Screen::GetScreen()
                            ->GetDisplayMatching(anchor_view_bounds_)
                            .bounds();

  // TODO(b/259440976): Calculate desired bounds based on anchor view bounds.
  gfx::Rect bounds = {
      {display_bounds.width() / 2 - 200, display_bounds.height() / 2 - 300},
      {400, 600}};
#if BUILDFLAG(IS_CHROMEOS_ASH)
  // For Ash, convert the position relative to the screen.
  // For Lacros, `bounds` is already relative to the toplevel window and the
  // position will be calculated on server side.
  wm::ConvertRectFromScreen(GetWidget()->GetNativeWindow()->parent(), &bounds);
#endif
  GetWidget()->SetBounds(bounds);
}

std::vector<views::View*> RichAnswersView::GetFocusableViews() {
  std::vector<views::View*> focusable_views;
  focusable_views.push_back(this);

  if (settings_button_ && settings_button_->GetVisible()) {
    focusable_views.push_back(settings_button_);
  }

  return focusable_views;
}

}  // namespace quick_answers
