// Copyright 2023 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef IOS_CHROME_BROWSER_AUTOFILL_BOTTOM_SHEET_AUTOFILL_BOTTOM_SHEET_TAB_HELPER_H_
#define IOS_CHROME_BROWSER_AUTOFILL_BOTTOM_SHEET_AUTOFILL_BOTTOM_SHEET_TAB_HELPER_H_

#import "components/autofill/core/browser/field_types.h"
#import "components/autofill/core/common/unique_ids.h"
#import "ios/web/public/web_state_observer.h"
#import "ios/web/public/web_state_user_data.h"

namespace autofill {
struct FormActivityParams;
class FormStructure;
}  // namespace autofill

namespace web {
class ScriptMessage;
class WebFrame;
}  // namespace web

@protocol AutofillBottomSheetCommands;
@class CommandDispatcher;
@protocol PasswordsAccountStorageNoticeHandler;

class AutofillBottomSheetTabHelper
    : public web::WebStateObserver,
      public web::WebStateUserData<AutofillBottomSheetTabHelper> {
 public:
  // Maximum number of times the password bottom sheet can be
  // dismissed before it gets disabled.
  static constexpr int kPasswordBottomSheetMaxDismissCount = 3;

  AutofillBottomSheetTabHelper(const AutofillBottomSheetTabHelper&) = delete;
  AutofillBottomSheetTabHelper& operator=(const AutofillBottomSheetTabHelper&) =
      delete;

  ~AutofillBottomSheetTabHelper() override;

  // Handler for JavaScript messages. Dispatch to more specific handler.
  void OnFormMessageReceived(const web::ScriptMessage& message);

  // Sets the bottom sheet CommandDispatcher.
  void SetAutofillBottomSheetHandler(
      id<AutofillBottomSheetCommands> commands_handler);

  // Prepare bottom sheet using data from the password form prediction.
  void AttachPasswordListeners(
      const std::vector<autofill::FieldRendererId>& renderer_ids,
      web::WebFrame* frame);

  // Prepare bottom sheet using data from the credit card form prediction.
  void AttachPaymentsListeners(
      const std::vector<autofill::FormStructure*>& forms,
      web::WebFrame* frame);

  // Detach the password listeners, which will deactivate the password bottom
  // sheet.
  void DetachPasswordListeners(web::WebFrame* frame, bool refocus);

  // Detach the payments listeners, which will deactivate the payments bottom
  // sheet.
  void DetachPaymentsListeners(web::WebFrame* frame, bool refocus);

  // WebStateObserver:
  void DidFinishNavigation(web::WebState* web_state,
                           web::NavigationContext* navigation_context) override;
  void WebStateDestroyed(web::WebState* web_state) override;

 private:
  friend class web::WebStateUserData<AutofillBottomSheetTabHelper>;

  explicit AutofillBottomSheetTabHelper(
      web::WebState* web_state,
      id<PasswordsAccountStorageNoticeHandler>
          password_account_storage_notice_handler);

  // Check whether the password bottom sheet has been dismissed too many times
  // by the user.
  bool HasReachedDismissLimit();

  // Prepare bottom sheet using data from the form prediction.
  void AttachListeners(
      const std::vector<autofill::FieldRendererId>& renderer_ids,
      std::set<autofill::FieldRendererId>& registered_renderer_ids,
      web::WebFrame* frame,
      bool must_be_empty);

  // Send command to show the Password Bottom Sheet.
  void ShowPasswordBottomSheet(const autofill::FormActivityParams params);

  // Send command to show the Payments Bottom Sheet.
  void ShowPaymentsBottomSheet(const autofill::FormActivityParams params);

  // Handler used to request showing the password bottom sheet.
  __weak id<AutofillBottomSheetCommands> commands_handler_;

  // Handler used for the passwords account storage notice.
  // TODO(crbug.com/1434606): Remove this when the move to account storage
  // notice is removed.
  __weak id<PasswordsAccountStorageNoticeHandler>
      password_account_storage_notice_handler_;

  // The WebState with which this object is associated.
  web::WebState* const web_state_;

  // List of password bottom sheet related renderer ids.
  std::set<autofill::FieldRendererId> registered_password_renderer_ids_;

  // List of payments bottom sheet related renderer ids.
  std::set<autofill::FieldRendererId> registered_payments_renderer_ids_;

  WEB_STATE_USER_DATA_KEY_DECL();
};

#endif  // IOS_CHROME_BROWSER_AUTOFILL_BOTTOM_SHEET_AUTOFILL_BOTTOM_SHEET_TAB_HELPER_H_
