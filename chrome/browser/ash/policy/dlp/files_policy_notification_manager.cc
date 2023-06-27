// Copyright 2023 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/browser/ash/policy/dlp/files_policy_notification_manager.h"

#include <cstddef>
#include <memory>
#include <string>

#include "ash/public/cpp/new_window_delegate.h"
#include "base/check.h"
#include "base/containers/contains.h"
#include "base/files/file_path.h"
#include "base/functional/bind.h"
#include "base/functional/callback_forward.h"
#include "base/functional/callback_helpers.h"
#include "base/memory/scoped_refptr.h"
#include "base/notreached.h"
#include "base/strings/string_number_conversions.h"
#include "chrome/browser/ash/extensions/file_manager/system_notification_manager.h"
#include "chrome/browser/ash/file_manager/io_task.h"
#include "chrome/browser/ash/file_manager/io_task_controller.h"
#include "chrome/browser/ash/file_manager/url_util.h"
#include "chrome/browser/ash/file_manager/volume_manager.h"
#include "chrome/browser/ash/policy/dlp/dialogs/files_policy_dialog.h"
#include "chrome/browser/ash/policy/dlp/dialogs/files_policy_error_dialog.h"
#include "chrome/browser/ash/policy/dlp/dialogs/files_policy_warn_dialog.h"
#include "chrome/browser/chromeos/policy/dlp/dialogs/policy_dialog_base.h"
#include "chrome/browser/chromeos/policy/dlp/dlp_confidential_file.h"
#include "chrome/browser/chromeos/policy/dlp/dlp_file_destination.h"
#include "chrome/browser/chromeos/policy/dlp/dlp_files_controller.h"
#include "chrome/browser/chromeos/policy/dlp/dlp_files_utils.h"
#include "chrome/browser/chromeos/policy/dlp/dlp_policy_constants.h"
#include "chrome/browser/notifications/notification_display_service.h"
#include "chrome/browser/notifications/notification_display_service_factory.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/ash/system_web_apps/system_web_app_ui_utils.h"
#include "chrome/browser/ui/browser_list.h"
#include "chrome/browser/ui/browser_window.h"
#include "chromeos/dbus/dlp/dlp_service.pb.h"
#include "components/strings/grit/components_strings.h"
#include "content/public/browser/browser_context.h"
#include "storage/browser/file_system/file_system_url.h"
#include "third_party/abseil-cpp/absl/types/optional.h"
#include "ui/base/l10n/l10n_util.h"
#include "ui/message_center/public/cpp/notification.h"
#include "ui/message_center/public/cpp/notification_delegate.h"

namespace policy {

namespace {
// How long to wait for a Files App to open before falling back to a system
// modal.
const base::TimeDelta kOpenFilesAppTimeout = base::Milliseconds(3000);

constexpr char kDlpFilesNotificationId[] = "dlp_files";

// TODO(b/279435843): Replace with translation strings.
std::u16string GetNotificationTitle(dlp::FileAction action,
                                    NotificationType type) {
  switch (action) {
    case dlp::FileAction::kDownload:
      return type == NotificationType::kError
                 ? l10n_util::GetStringUTF16(
                       IDS_POLICY_DLP_FILES_DOWNLOAD_BLOCK_TITLE)
                 : u"Review is required before downloading";
    case dlp::FileAction::kUpload:
      return type == NotificationType::kError
                 ? l10n_util::GetStringUTF16(
                       IDS_POLICY_DLP_FILES_UPLOAD_BLOCK_TITLE)
                 : u"Review is required before uploading";
    case dlp::FileAction::kOpen:
    case dlp::FileAction::kShare:
      return type == NotificationType::kError
                 ? l10n_util::GetStringUTF16(
                       IDS_POLICY_DLP_FILES_OPEN_BLOCK_TITLE)
                 : u"Review is required before opening";
    case dlp::FileAction::kCopy:
      return type == NotificationType::kError
                 ? u"Blocked copy"
                 : u"Review is required before copying";
    case dlp::FileAction::kMove:
      return type == NotificationType::kError
                 ? u"Blocked move"
                 : u"Review is required before moving";
    case dlp::FileAction::kTransfer:
      return type == NotificationType::kError
                 ? u"Blocked transfer"
                 : u"Review is required before transferring";
    case dlp::FileAction::kUnknown:
      NOTREACHED_NORETURN();
  }
}

// TODO(b/279435843): Replace with translation strings.
std::u16string GetNotificationMessage(size_t file_count,
                                      NotificationType type) {
  if (type == NotificationType::kError) {
    return file_count == 1 ? u"File was blocked"
                           : u"Review for further details";
  }
  // kWarning
  return file_count == 1 ? u"File may contain sensitive content"
                         : u"Files may contain sensitive content";
}

// TODO(b/279435843): Replace with translation strings.
std::u16string GetOkButton(dlp::FileAction action,
                           size_t file_count,
                           NotificationType type) {
  // Multiple files - both warnings and errors have a Review button.
  if (file_count > 1) {
    return u"Review";
  }
  // Single file - button text depends on the type.
  if (type == NotificationType::kError) {
    return l10n_util::GetStringUTF16(IDS_LEARN_MORE);
  }

  DCHECK(type == NotificationType::kWarning);
  switch (action) {
    case dlp::FileAction::kDownload:
      return l10n_util::GetStringUTF16(
          IDS_POLICY_DLP_FILES_DOWNLOAD_WARN_CONTINUE_BUTTON);
    case dlp::FileAction::kUpload:
      return l10n_util::GetStringUTF16(
          IDS_POLICY_DLP_FILES_UPLOAD_WARN_CONTINUE_BUTTON);
    case dlp::FileAction::kCopy:
      return l10n_util::GetStringUTF16(
          IDS_POLICY_DLP_FILES_COPY_WARN_CONTINUE_BUTTON);
    case dlp::FileAction::kMove:
      return l10n_util::GetStringUTF16(
          IDS_POLICY_DLP_FILES_MOVE_WARN_CONTINUE_BUTTON);
    case dlp::FileAction::kOpen:
    case dlp::FileAction::kShare:
      return l10n_util::GetStringUTF16(
          IDS_POLICY_DLP_FILES_OPEN_WARN_CONTINUE_BUTTON);
    case dlp::FileAction::kTransfer:
    case dlp::FileAction::kUnknown:
      // TODO(crbug.com/1361900): Set proper text when file action is unknown.
      return l10n_util::GetStringUTF16(
          IDS_POLICY_DLP_FILES_TRANSFER_WARN_CONTINUE_BUTTON);
  }
}

// TODO(b/279435843): Replace with translation strings.
std::u16string GetCancelButton(NotificationType type) {
  switch (type) {
    case NotificationType::kError:
      return u"Dismiss";
    case NotificationType::kWarning:
      return l10n_util::GetStringUTF16(IDS_POLICY_DLP_WARN_CANCEL_BUTTON);
  }
}

// TODO(b/279435843): Replace with translation strings.
std::u16string GetTimeoutNotificationTitle(dlp::FileAction action) {
  switch (action) {
    case dlp::FileAction::kDownload:
      return u"Download cancelled";
    case dlp::FileAction::kTransfer:
    case dlp::FileAction::kUnknown:
      // TODO(crbug.com/1361900): Set proper text when file action is unknown.
      return u"Transfer cancelled";
    case dlp::FileAction::kUpload:
      return u"Upload cancelled";
    case dlp::FileAction::kCopy:
      return u"Copy cancelled";
    case dlp::FileAction::kMove:
      return u"Move cancelled";
    case dlp::FileAction::kOpen:
    case dlp::FileAction::kShare:
      return u"Open cancelled";
  }
}

// TODO(b/279435843): Replace with translation strings.
std::u16string GetTimeoutNotificationMessage(dlp::FileAction action) {
  switch (action) {
    case dlp::FileAction::kDownload:
      return u"Confirmation was required to continue downloading your files. "
             u"Please try again";
    case dlp::FileAction::kTransfer:
    case dlp::FileAction::kUnknown:
      // TODO(crbug.com/1361900): Set proper text when file action is unknown.
      return u"Confirmation was required to continue transferring your files. "
             u"Please try again";
    case dlp::FileAction::kUpload:
      return u"Confirmation was required to continue uploading your files. "
             u"Please try again";
    case dlp::FileAction::kCopy:
      return u"Confirmation was required to continue copying your files. "
             u"Please try again";
    case dlp::FileAction::kMove:
      return u"Confirmation was required to continue moving your files. "
             u"Please try again";
    case dlp::FileAction::kOpen:
    case dlp::FileAction::kShare:
      return u"Confirmation was required to continue opening your files. "
             u"Please try again";
  }
}

// Dismisses the notification with `notification_id`.
void Dismiss(content::BrowserContext* context,
             const std::string& notification_id) {
  auto* profile = Profile::FromBrowserContext(context);
  DCHECK(profile);
  NotificationDisplayServiceFactory::GetForProfile(profile)->Close(
      NotificationHandler::Type::TRANSIENT, notification_id);
}

file_manager::io_task::IOTaskController* GetIOTaskController(
    content::BrowserContext* context) {
  DCHECK(context);
  file_manager::VolumeManager* const volume_manager =
      file_manager::VolumeManager::Get(Profile::FromBrowserContext(context));
  if (!volume_manager) {
    LOG(ERROR) << "FilesPolicyNotificationManager failed to find "
                  "file_manager::VolumeManager";
    return nullptr;
  }
  return volume_manager->io_task_controller();
}

// Computes and returns a new notification ID by appending `count` to the
// prefix.
std::string GetNotificationId(size_t count) {
  return kDlpFilesNotificationId + std::string("_") +
         base::NumberToString(count);
}

// Notification click handler implementation for files policy notifications.
// The handler ensures that we only handle the button click once. This is
// required because some of the parameters are move-only types and wouldn't be
// valid on the second invocation.
class PolicyNotificationClickHandler
    : public message_center::NotificationDelegate {
 public:
  explicit PolicyNotificationClickHandler(
      base::OnceCallback<void(absl::optional<int>)> callback)
      : callback_(std::move(callback)) {}

  // message_center::NotificationDelegate overrides:
  void Click(const absl::optional<int>& button_index,
             const absl::optional<std::u16string>& reply) override {
    if (!button_index.has_value()) {
      // Ignore clicks on the notification, but not on the buttons.
      return;
    }

    if (callback_) {
      std::move(callback_).Run(button_index);
    }
  }

 private:
  ~PolicyNotificationClickHandler() override = default;

  base::OnceCallback<void(absl::optional<int>)> callback_;
};

}  // namespace

FilesPolicyNotificationManager::FilesPolicyNotificationManager(
    content::BrowserContext* context)
    : context_(context),
      task_runner_(base::SequencedTaskRunner::GetCurrentDefault()) {
  DCHECK(context);

  auto* io_task_controller = GetIOTaskController(context_);
  if (!io_task_controller) {
    LOG(ERROR) << "FilesPolicyNotificationManager failed to find "
                  "file_manager::io_task::IOTaskController";
    return;
  }
  io_task_controller->AddObserver(this);
}

FilesPolicyNotificationManager::~FilesPolicyNotificationManager() = default;

void FilesPolicyNotificationManager::ShowDlpBlockedFiles(
    absl::optional<file_manager::io_task::IOTaskId> task_id,
    std::vector<base::FilePath> blocked_files,
    dlp::FileAction action) {
  // If `task_id` has value, the corresponding IOTask should be updated
  // accordingly.
  if (task_id.has_value()) {
    if (!HasIOTask(task_id.value())) {
      // Task already completed and removed.
      return;
    }
    for (const auto& file : blocked_files) {
      io_tasks_.at(task_id.value())
          .blocked_files.emplace(DlpConfidentialFile(file), Policy::kDlp);
    }
  } else {
    ShowDlpBlockNotification(std::move(blocked_files), action);
  }
}

void FilesPolicyNotificationManager::ShowDlpWarning(
    OnDlpRestrictionCheckedCallback callback,
    absl::optional<file_manager::io_task::IOTaskId> task_id,
    std::vector<base::FilePath> warning_files,
    const DlpFileDestination& destination,
    dlp::FileAction action) {
  // If `task_id` has value, the corresponding IOTask should be paused.
  if (task_id.has_value()) {
    PauseIOTask(task_id.value(), std::move(callback), std::move(warning_files),
                action, Policy::kDlp);
  } else {
    ShowDlpWarningNotification(std::move(callback), std::move(warning_files),
                               destination, action);
  }
}

void FilesPolicyNotificationManager::ShowsFilesPolicyNotification(
    const std::string& notification_id,
    const file_manager::io_task::ProgressStatus& status) {
  const dlp::FileAction action =
      status.type == file_manager::io_task::OperationType::kCopy
          ? dlp::FileAction::kCopy
          : dlp::FileAction::kMove;

  if (status.HasPolicyError() &&
      status.policy_error ==
          file_manager::io_task::PolicyErrorType::kDlpWarningTimeout) {
    ShowDlpWarningTimeoutNotification(action, notification_id);
    return;
  }

  const file_manager::io_task::IOTaskId id(status.task_id);
  auto callback =
      status.HasWarning()
          ? base::BindRepeating(&FilesPolicyNotificationManager::
                                    HandleFilesPolicyWarningNotificationClick,
                                weak_factory_.GetWeakPtr(), id, notification_id)
          : base::BindRepeating(&FilesPolicyNotificationManager::
                                    HandleFilesPolicyErrorNotificationClick,
                                weak_factory_.GetWeakPtr(), id,
                                notification_id);
  // The notification should stay visible until actioned upon.
  message_center::RichNotificationData optional_fields;
  optional_fields.never_timeout = true;
  const NotificationType type = status.HasWarning() ? NotificationType::kWarning
                                                    : NotificationType::kError;
  // TODO(aidazolic): Use # warned/blocked files for strings, not total.
  const size_t file_count = status.sources.size();
  auto notification = file_manager::CreateSystemNotification(
      notification_id, GetNotificationTitle(action, type),
      GetNotificationMessage(status.sources.size(), type),
      base::MakeRefCounted<message_center::HandleNotificationClickDelegate>(
          std::move(callback)),
      optional_fields);
  notification->set_buttons(
      {message_center::ButtonInfo(GetCancelButton(type)),
       message_center::ButtonInfo(GetOkButton(action, file_count, type))});
  auto* profile = Profile::FromBrowserContext(context_);
  DCHECK(profile);
  NotificationDisplayServiceFactory::GetForProfile(profile)->Display(
      NotificationHandler::Type::TRANSIENT, *notification,
      /*metadata=*/nullptr);
}

void FilesPolicyNotificationManager::ShowDialog(
    file_manager::io_task::IOTaskId task_id,
    FilesDialogType type) {
  auto* profile = Profile::FromBrowserContext(context_);
  DCHECK(profile);

  // Get the last active Files app window.
  Browser* browser =
      FindSystemWebAppBrowser(profile, ash::SystemWebAppType::FILE_MANAGER);
  gfx::NativeWindow modal_parent =
      browser ? browser->window()->GetNativeWindow() : nullptr;
  if (modal_parent) {
    ShowDialogForIOTask(task_id, type, modal_parent);
    return;
  }

  // No window found, so open a new one. This should notify us by through
  // OnBrowserSetLastActive() to show the dialog.
  LaunchFilesApp(std::make_unique<DialogInfo>(
      base::BindOnce(&FilesPolicyNotificationManager::ShowDialogForIOTask,
                     weak_factory_.GetWeakPtr(), task_id, type),
      task_id,
      base::BindOnce(&FilesPolicyNotificationManager::OnIOTaskTimedOut,
                     weak_factory_.GetWeakPtr(), task_id)));
}

void FilesPolicyNotificationManager::ShowDlpWarningTimeoutNotification(
    dlp::FileAction action,
    absl::optional<std::string> notification_id) {
  if (!notification_id.has_value()) {
    notification_id = GetNotificationId(notification_count_++);
  }
  // The notification should stay visible until dismissed.
  message_center::RichNotificationData optional_fields;
  optional_fields.never_timeout = true;
  auto notification = file_manager::CreateSystemNotification(
      notification_id.value(), GetTimeoutNotificationTitle(action),
      GetTimeoutNotificationMessage(action),
      base::MakeRefCounted<message_center::HandleNotificationClickDelegate>(
          base::BindRepeating(&Dismiss, context_, notification_id.value())),
      optional_fields);
  notification->set_buttons(
      {message_center::ButtonInfo(GetCancelButton(NotificationType::kError))});
  auto* profile = Profile::FromBrowserContext(context_);
  DCHECK(profile);
  NotificationDisplayServiceFactory::GetForProfile(profile)->Display(
      NotificationHandler::Type::TRANSIENT, *notification,
      /*metadata=*/nullptr);
}

bool FilesPolicyNotificationManager::HasIOTask(
    file_manager::io_task::IOTaskId task_id) const {
  return base::Contains(io_tasks_, task_id);
}

void FilesPolicyNotificationManager::OnIOTaskResumed(
    file_manager::io_task::IOTaskId task_id) {
  if (!HasIOTask(task_id)) {
    // Task is already completed or timed out.
    return;
  }

  if (!HasWarning(task_id)) {
    // Warning callback is already run.
    return;
  }

  std::move(io_tasks_.at(task_id).warning_info->warning_callback)
      .Run(/*should_proceed=*/true);
  io_tasks_.at(task_id).warning_info.reset();
}

std::map<DlpConfidentialFile, Policy>
FilesPolicyNotificationManager::GetIOTaskBlockedFilesForTesting(
    file_manager::io_task::IOTaskId task_id) const {
  if (!HasIOTask(task_id)) {
    return {};
  }
  return io_tasks_.at(task_id).blocked_files;
}

void FilesPolicyNotificationManager::HandleDlpWarningNotificationClick(
    std::string notification_id,
    OnDlpRestrictionCheckedCallback callback,
    std::vector<DlpConfidentialFile> files,
    DlpFileDestination destination,
    dlp::FileAction action,
    absl::optional<int> button_index) {
  if (!button_index.has_value()) {
    return;
  }

  if (button_index == NotificationButton::CANCEL) {
    std::move(callback).Run(/*should_proceed=*/false);
  }

  if (button_index == NotificationButton::OK) {
    DCHECK(files.size() >= 1);

    if (files.size() == 1) {
      // Action anyway.
      std::move(callback).Run(/*should_proceed=*/true);
    } else {
      // Review.
      FileTaskInfo info(action);
      info.destination = destination;
      info.warning_info.emplace(
          std::move(files), Policy::kDlp, std::move(callback),
          base::BindOnce(
              &FilesPolicyNotificationManager::OnNonIOTaskWarningDialogClicked,
              weak_factory_.GetWeakPtr(), notification_id, Policy::kDlp));
      non_io_tasks_.emplace(notification_id, std::move(info));
      // Always open the Files app. This should notify us by through
      // OnBrowserSetLastActive() to show the dialog.
      LaunchFilesApp(std::make_unique<DialogInfo>(
          base::BindOnce(
              &FilesPolicyNotificationManager::ShowDialogForNonIOTask,
              weak_factory_.GetWeakPtr(), notification_id,
              FilesDialogType::kWarning),
          notification_id,
          base::BindOnce(&FilesPolicyNotificationManager::OnNonIOTaskTimedOut,
                         weak_factory_.GetWeakPtr(), notification_id)));
    }
  }

  // Dismiss the notification.
  Dismiss(context_, notification_id);
}

void FilesPolicyNotificationManager::HandleDlpErrorNotificationClick(
    std::string notification_id,
    std::vector<DlpConfidentialFile> files,
    dlp::FileAction action,
    absl::optional<int> button_index) {
  if (!button_index.has_value()) {
    return;
  }

  if (button_index == NotificationButton::CANCEL) {
    Dismiss(context_, notification_id);
  }

  if (button_index == NotificationButton::OK) {
    DCHECK(files.size() >= 1);

    if (files.size() == 1) {
      // Learn more.
      ash::NewWindowDelegate::GetPrimary()->OpenUrl(
          GURL(dlp::kDlpLearnMoreUrl),
          ash::NewWindowDelegate::OpenUrlFrom::kUserInteraction,
          ash::NewWindowDelegate::Disposition::kNewForegroundTab);
    } else {
      // Review.
      FileTaskInfo info(action);
      for (const auto& file : files) {
        info.blocked_files.emplace(DlpConfidentialFile(file.file_path),
                                   Policy::kDlp);
      }
      non_io_tasks_.emplace(notification_id, std::move(info));
      // Always open the Files app. This should notify us by through
      // OnBrowserSetLastActive() to show the dialog.
      LaunchFilesApp(std::make_unique<DialogInfo>(
          base::BindOnce(
              &FilesPolicyNotificationManager::ShowDialogForNonIOTask,
              weak_factory_.GetWeakPtr(), notification_id,
              FilesDialogType::kError),
          notification_id,
          base::BindOnce(&FilesPolicyNotificationManager::OnNonIOTaskTimedOut,
                         weak_factory_.GetWeakPtr(), notification_id)));
    }
  }

  // Dismiss the notification.
  Dismiss(context_, notification_id);
}

FilesPolicyNotificationManager::WarningInfo::WarningInfo(
    std::vector<base::FilePath> files_paths,
    Policy warning_reason,
    OnDlpRestrictionCheckedCallback warning_callback,
    OnDlpRestrictionCheckedCallback dialog_callback)
    : warning_reason(warning_reason),
      warning_callback(std::move(warning_callback)),
      dialog_callback(std::move(dialog_callback)) {
  for (const auto& file_path : files_paths) {
    files.emplace_back(file_path);
  }
}

FilesPolicyNotificationManager::WarningInfo::WarningInfo(
    std::vector<DlpConfidentialFile> files,
    Policy warning_reason,
    OnDlpRestrictionCheckedCallback warning_callback,
    OnDlpRestrictionCheckedCallback dialog_callback)
    : files(std::move(files)),
      warning_reason(warning_reason),
      warning_callback(std::move(warning_callback)),
      dialog_callback(std::move(dialog_callback)) {}

FilesPolicyNotificationManager::WarningInfo::WarningInfo(WarningInfo&& other) =
    default;

FilesPolicyNotificationManager::WarningInfo::~WarningInfo() = default;

FilesPolicyNotificationManager::FileTaskInfo::FileTaskInfo(
    dlp::FileAction action)
    : action(action) {}

FilesPolicyNotificationManager::FileTaskInfo::FileTaskInfo(
    FileTaskInfo&& other) = default;

FilesPolicyNotificationManager::FileTaskInfo::~FileTaskInfo() = default;

FilesPolicyNotificationManager::DialogInfo::DialogInfo(
    ShowDialogCallback dialog_callback,
    file_manager::io_task::IOTaskId task_id,
    base::OnceClosure timeout_callback)
    : task_id(task_id),
      notification_id(absl::nullopt),
      dialog_callback(std::move(dialog_callback)),
      timeout_callback(std::move(timeout_callback)) {}

FilesPolicyNotificationManager::DialogInfo::DialogInfo(
    ShowDialogCallback dialog_callback,
    std::string notification_id,
    base::OnceClosure timeout_callback)
    : task_id(absl::nullopt),
      notification_id(notification_id),
      dialog_callback(std::move(dialog_callback)),
      timeout_callback(std::move(timeout_callback)) {}

FilesPolicyNotificationManager::DialogInfo::~DialogInfo() = default;

void FilesPolicyNotificationManager::HandleFilesPolicyWarningNotificationClick(
    file_manager::io_task::IOTaskId task_id,
    std::string notification_id,
    absl::optional<int> button_index) {
  if (!button_index.has_value()) {
    return;
  }
  if (!HasIOTask(task_id)) {
    // Task already completed.
    return;
  }
  if (!HasWarning(task_id)) {
    LOG(WARNING) << "Warning notification clicked but no warning info found";
    return;
  }

  if (button_index.value() == NotificationButton::CANCEL) {
    Cancel(task_id);
    Dismiss(context_, notification_id);
  }

  if (button_index.value() == NotificationButton::OK) {
    if (io_tasks_.at(task_id).warning_info->files.size() == 1) {
      // Single file - proceed.
      Resume(task_id);
    } else {
      // Multiple files - review.
      ShowDialog(task_id, FilesDialogType::kWarning);
    }
    Dismiss(context_, notification_id);
  }
}

void FilesPolicyNotificationManager::HandleFilesPolicyErrorNotificationClick(
    file_manager::io_task::IOTaskId task_id,
    std::string notification_id,
    absl::optional<int> button_index) {
  if (!button_index.has_value()) {
    return;
  }
  if (!HasIOTask(task_id)) {
    // Task already completed.
    return;
  }
  if (!HasBlockedFiles(task_id)) {
    LOG(WARNING) << "Error notification clicked but no blocked files found";
    return;
  }

  if (button_index.value() == NotificationButton::CANCEL) {
    Dismiss(context_, notification_id);
  }

  if (button_index.value() == NotificationButton::OK) {
    if (io_tasks_.at(task_id).blocked_files.size() == 1) {
      // Single file - open help page.
      // TODO(b/283786134): Open page based on policy.
      ash::NewWindowDelegate::GetPrimary()->OpenUrl(
          GURL(dlp::kDlpLearnMoreUrl),
          ash::NewWindowDelegate::OpenUrlFrom::kUserInteraction,
          ash::NewWindowDelegate::Disposition::kNewForegroundTab);
    } else {
      // Multiple files - review.
      ShowDialog(task_id, FilesDialogType::kError);
    }
  }
  Dismiss(context_, notification_id);
  io_tasks_.erase(task_id);
}

void FilesPolicyNotificationManager::ShowDialogForIOTask(
    file_manager::io_task::IOTaskId task_id,
    FilesDialogType type,
    gfx::NativeWindow modal_parent) {
  if (!HasIOTask(task_id)) {
    // Task already completed or timed out.
    return;
  }

  ShowFilesPolicyDialog(std::ref(io_tasks_.at(task_id)), type, modal_parent);
  if (type == FilesDialogType::kError) {
    io_tasks_.erase(task_id);
  }
}

void FilesPolicyNotificationManager::ShowDialogForNonIOTask(
    std::string notification_id,
    FilesDialogType type,
    gfx::NativeWindow modal_parent) {
  if (!HasNonIOTask(notification_id)) {
    // Task already completed or timed out.
    return;
  }
  ShowFilesPolicyDialog(std::ref(non_io_tasks_.at(notification_id)), type,
                        modal_parent);

  if (type == FilesDialogType::kError) {
    non_io_tasks_.erase(notification_id);
  }
}

void FilesPolicyNotificationManager::ShowFilesPolicyDialog(
    FileTaskInfo& info,
    FilesDialogType type,
    gfx::NativeWindow modal_parent) {
  switch (type) {
    case FilesDialogType::kUnknown:
      LOG(WARNING) << "Unknown FilesDialogType passed";
      return;
    case FilesDialogType::kError:
      if (info.blocked_files.empty()) {
        return;
      }
      // TODO(b/285568353): Remove destination.
      FilesPolicyDialog::CreateErrorDialog(
          info.blocked_files,
          info.destination.value_or(DlpFileDestination("https://example.com")),
          info.action, modal_parent);
      break;
    case FilesDialogType::kWarning:
      if (!info.warning_info.has_value()) {
        return;
      }
      CHECK(!info.warning_info->warning_callback.is_null());
      FilesPolicyDialog::CreateWarnDialog(
          std::move(info.warning_info->dialog_callback),
          info.warning_info->files,
          info.destination.value_or(DlpFileDestination("https://example.com")),
          info.action, modal_parent);
      break;
  }
  // TODO(ayaelattar): Timeout after total 5 minutes.
}

void FilesPolicyNotificationManager::AddIOTask(
    file_manager::io_task::IOTaskId task_id,
    dlp::FileAction action) {
  io_tasks_.emplace(std::move(task_id), FileTaskInfo(action));
}

void FilesPolicyNotificationManager::LaunchFilesApp(
    std::unique_ptr<DialogInfo> info) {
  // Start observing the browser list only if the queue is empty.
  if (pending_dialogs_.empty()) {
    BrowserList::AddObserver(this);
  }
  StartTimer(info.get(), std::move(info->timeout_callback));
  pending_dialogs_.emplace(std::move(info));

  ui::SelectFileDialog::FileTypeInfo file_type_info;
  file_type_info.allowed_paths =
      ui::SelectFileDialog::FileTypeInfo::ANY_PATH_OR_URL;
  GURL files_swa_url = file_manager::util::GetFileManagerMainPageUrlWithParams(
      ui::SelectFileDialog::SELECT_NONE,
      /*title=*/{},
      /*current_directory_url=*/{},
      /*selection_url=*/{},
      /*target_name=*/{}, &file_type_info,
      /*file_type_index=*/0,
      /*search_query=*/{},
      /*show_android_picker_apps=*/false,
      /*volume_filter=*/{});
  ash::SystemAppLaunchParams params;
  params.url = files_swa_url;
  ash::LaunchSystemWebAppAsync(Profile::FromBrowserContext(context_),
                               ash::SystemWebAppType::FILE_MANAGER, params);
}

void FilesPolicyNotificationManager::OnBrowserSetLastActive(Browser* browser) {
  if (!ash::IsBrowserForSystemWebApp(browser,
                                     ash::SystemWebAppType::FILE_MANAGER)) {
    LOG(WARNING) << "Browser did not match Files app";
    return;
  }

  // Files app successfully opened.
  ShowPendingDialog(browser->window()->GetNativeWindow());
}

void FilesPolicyNotificationManager::SetTaskRunnerForTesting(
    scoped_refptr<base::SequencedTaskRunner> task_runner) {
  task_runner_ = task_runner;
}

void FilesPolicyNotificationManager::OnIOTaskStatus(
    const file_manager::io_task::ProgressStatus& status) {
  // Observe only Copy and Move tasks.
  if (status.type != file_manager::io_task::OperationType::kCopy &&
      status.type != file_manager::io_task::OperationType::kMove) {
    return;
  }

  dlp::FileAction action =
      status.type == file_manager::io_task::OperationType::kCopy
          ? dlp::FileAction::kCopy
          : dlp::FileAction::kMove;

  if (!HasIOTask(status.task_id) &&
      status.state == file_manager::io_task::State::kQueued) {
    AddIOTask(status.task_id, action);
    return;
  }

  if (HasIOTask(status.task_id) && status.IsCompleted()) {
    if (status.state == file_manager::io_task::State::kCancelled &&
        HasWarning(status.task_id)) {
      CHECK(!io_tasks_.at(status.task_id)
                 .warning_info->warning_callback.is_null());
      std::move(io_tasks_.at(status.task_id).warning_info->warning_callback)
          .Run(/*should_proceed=*/false);
      io_tasks_.at(status.task_id).warning_info.reset();
    }
    // Remove only if the IOTask doesn't have any blocked file.
    if (!HasBlockedFiles(status.task_id)) {
      io_tasks_.erase(status.task_id);
    }
  }
}

bool FilesPolicyNotificationManager::HasBlockedFiles(
    file_manager::io_task::IOTaskId task_id) const {
  return HasIOTask(task_id) && !io_tasks_.at(task_id).blocked_files.empty();
}

bool FilesPolicyNotificationManager::HasWarning(
    file_manager::io_task::IOTaskId task_id) const {
  return HasIOTask(task_id) && io_tasks_.at(task_id).warning_info.has_value();
}

bool FilesPolicyNotificationManager::HasNonIOTask(
    const std::string notification_id) const {
  return base::Contains(non_io_tasks_, notification_id);
}

bool FilesPolicyNotificationManager::HasBlockedFiles(
    const std::string notification_id) const {
  return HasNonIOTask(notification_id) &&
         !non_io_tasks_.at(notification_id).blocked_files.empty();
}

bool FilesPolicyNotificationManager::HasWarning(
    const std::string notification_id) const {
  return HasNonIOTask(notification_id) &&
         non_io_tasks_.at(notification_id).warning_info.has_value();
}

void FilesPolicyNotificationManager::OnIOTaskWarningDialogClicked(
    file_manager::io_task::IOTaskId task_id,
    Policy warning_reason,
    bool should_proceed) {
  if (!HasIOTask(task_id) || !HasWarning(task_id)) {
    // Task probably timed out.
    return;
  }
  if (should_proceed) {
    Resume(task_id);
  } else {
    Cancel(task_id);
  }
}

void FilesPolicyNotificationManager::OnNonIOTaskWarningDialogClicked(
    const std::string& notification_id,
    Policy warning_reason,
    bool should_proceed) {
  if (!HasWarning(notification_id)) {
    // Task probably timed out.
    return;
  }
  std::move(non_io_tasks_.at(notification_id).warning_info->warning_callback)
      .Run(should_proceed);
  non_io_tasks_.erase(notification_id);
}

void FilesPolicyNotificationManager::OnLearnMoreButtonClicked(
    const std::string& notification_id,
    absl::optional<int> button_index) {
  if (!button_index || button_index.value() != 0) {
    return;
  }

  ash::NewWindowDelegate::GetPrimary()->OpenUrl(
      GURL(dlp::kDlpLearnMoreUrl),
      ash::NewWindowDelegate::OpenUrlFrom::kUserInteraction,
      ash::NewWindowDelegate::Disposition::kNewForegroundTab);

  Dismiss(context_, notification_id);
}

void FilesPolicyNotificationManager::Resume(
    file_manager::io_task::IOTaskId task_id) {
  if (!HasIOTask(task_id) || !HasWarning(task_id)) {
    return;
  }
  auto* io_task_controller = GetIOTaskController(context_);
  if (!io_task_controller) {
    LOG(ERROR) << "FilesPolicyNotificationManager failed to find "
                  "file_manager::io_task::IOTaskController";
    return;
  }
  file_manager::io_task::ResumeParams params;
  params.policy_params = file_manager::io_task::PolicyResumeParams(
      io_tasks_.at(task_id).warning_info->warning_reason);
  io_task_controller->Resume(task_id, std::move(params));
}

void FilesPolicyNotificationManager::Cancel(
    file_manager::io_task::IOTaskId task_id) {
  if (!HasIOTask(task_id) || !HasWarning(task_id)) {
    return;
  }
  auto* io_task_controller = GetIOTaskController(context_);
  if (!io_task_controller) {
    LOG(ERROR) << "FilesPolicyNotificationManager failed to find "
                  "file_manager::io_task::IOTaskController";
    return;
  }
  io_task_controller->Cancel(task_id);
}

void FilesPolicyNotificationManager::Shutdown() {
  file_manager::VolumeManager* const volume_manager =
      file_manager::VolumeManager::Get(Profile::FromBrowserContext(context_));
  if (volume_manager) {
    auto* io_task_controller = volume_manager->io_task_controller();
    if (io_task_controller) {
      io_task_controller->RemoveObserver(this);
    }
  }
}

void FilesPolicyNotificationManager::ShowDlpBlockNotification(
    std::vector<base::FilePath> blocked_files,
    dlp::FileAction action) {
  const std::string notification_id = GetNotificationId(notification_count_++);
  std::unique_ptr<message_center::Notification> notification;

  if (DlpFilesController::kNewFilesPolicyUXEnabled) {
    // The notification should stay visible until actioned upon.
    message_center::RichNotificationData optional_fields;
    optional_fields.never_timeout = true;
    notification = file_manager::CreateSystemNotification(
        notification_id, GetNotificationTitle(action, NotificationType::kError),
        GetNotificationMessage(blocked_files.size(), NotificationType::kError),
        base::MakeRefCounted<PolicyNotificationClickHandler>(base::BindOnce(
            &FilesPolicyNotificationManager::HandleDlpErrorNotificationClick,
            weak_factory_.GetWeakPtr(), notification_id,
            std::vector<DlpConfidentialFile>(blocked_files.begin(),
                                             blocked_files.end()),
            action)),
        optional_fields);
    notification->set_buttons(
        {message_center::ButtonInfo(GetCancelButton(NotificationType::kError)),
         message_center::ButtonInfo(GetOkButton(action, blocked_files.size(),
                                                NotificationType::kError))});
  } else {
    std::u16string title;
    std::u16string message;
    switch (action) {
      case dlp::FileAction::kDownload:
        title = l10n_util::GetStringUTF16(
            IDS_POLICY_DLP_FILES_DOWNLOAD_BLOCK_TITLE);
        // ignore `blocked_files.size()` for downloads.
        message = l10n_util::GetStringUTF16(
            IDS_POLICY_DLP_FILES_DOWNLOAD_BLOCK_MESSAGE);
        break;
      case dlp::FileAction::kUpload:
        title =
            l10n_util::GetStringUTF16(IDS_POLICY_DLP_FILES_UPLOAD_BLOCK_TITLE);
        message = l10n_util::GetPluralStringFUTF16(
            IDS_POLICY_DLP_FILES_UPLOAD_BLOCK_MESSAGE, blocked_files.size());
        break;
      case dlp::FileAction::kOpen:
      case dlp::FileAction::kShare:
        title =
            l10n_util::GetStringUTF16(IDS_POLICY_DLP_FILES_OPEN_BLOCK_TITLE);
        message = l10n_util::GetPluralStringFUTF16(
            IDS_POLICY_DLP_FILES_OPEN_BLOCK_MESSAGE, blocked_files.size());
        break;
      case dlp::FileAction::kCopy:
      case dlp::FileAction::kMove:
      case dlp::FileAction::kTransfer:
      case dlp::FileAction::kUnknown:
        // TODO(b/269609831): Show correct notification here.
        return;
    }
    notification = file_manager::CreateSystemNotification(
        notification_id, title, message,
        base::MakeRefCounted<message_center::HandleNotificationClickDelegate>(
            base::BindRepeating(
                &FilesPolicyNotificationManager::OnLearnMoreButtonClicked,
                weak_factory_.GetWeakPtr(), notification_id)));
    notification->set_buttons({message_center::ButtonInfo(
        l10n_util::GetStringUTF16(IDS_LEARN_MORE))});
  }

  auto* profile = Profile::FromBrowserContext(context_);
  DCHECK(profile);
  NotificationDisplayServiceFactory::GetForProfile(profile)->Display(
      NotificationHandler::Type::TRANSIENT, *notification,
      /*metadata=*/nullptr);
}

void FilesPolicyNotificationManager::ShowDlpWarningNotification(
    OnDlpRestrictionCheckedCallback callback,
    std::vector<base::FilePath> warning_files,
    const DlpFileDestination& destination,
    dlp::FileAction action) {
  if (DlpFilesController::kNewFilesPolicyUXEnabled) {
    std::vector<DlpConfidentialFile> files = std::vector<DlpConfidentialFile>{
        warning_files.begin(), warning_files.end()};
    const std::string& notification_id =
        GetNotificationId(notification_count_++);
    std::vector<message_center::ButtonInfo> buttons = {
        message_center::ButtonInfo(GetCancelButton(NotificationType::kWarning)),
        message_center::ButtonInfo(
            GetOkButton(action, files.size(), NotificationType::kWarning))};
    // The notification should stay visible until actioned upon.
    message_center::RichNotificationData optional_fields;
    optional_fields.never_timeout = true;
    auto notification = file_manager::CreateSystemNotification(
        notification_id,
        GetNotificationTitle(action, NotificationType::kWarning),
        GetNotificationMessage(files.size(), NotificationType::kWarning),
        base::MakeRefCounted<PolicyNotificationClickHandler>(base::BindOnce(
            &FilesPolicyNotificationManager::HandleDlpWarningNotificationClick,
            weak_factory_.GetWeakPtr(), std::move(notification_id),
            std::move(callback), std::move(files), destination, action)));
    notification->set_buttons(std::move(buttons));

    auto* profile = Profile::FromBrowserContext(context_);
    DCHECK(profile);
    NotificationDisplayServiceFactory::GetForProfile(profile)->Display(
        NotificationHandler::Type::TRANSIENT, *notification,
        /*metadata=*/nullptr);
  } else {
    FilesPolicyDialog::CreateWarnDialog(
        std::move(callback),
        std::vector<DlpConfidentialFile>{warning_files.begin(),
                                         warning_files.end()},
        destination, action,
        /*modal_parent=*/nullptr);
  }
  // TODO(ayaelattar): Timeout after total 5 minutes.
}

void FilesPolicyNotificationManager::PauseIOTask(
    file_manager::io_task::IOTaskId task_id,
    OnDlpRestrictionCheckedCallback callback,
    std::vector<base::FilePath> warning_files,
    dlp::FileAction action,
    Policy warning_reason) {
  auto* io_task_controller = GetIOTaskController(context_);
  if (!io_task_controller) {
    // Proceed because the IO task can't be paused.
    std::move(callback).Run(/*should_proceed=*/true);
    return;
  }
  // Sometimes DLP checks are done before FilesPolicyNotificationManager is
  // lazily created, so the task is not tracked and the pausing won't happen.
  if (!HasIOTask(task_id)) {
    AddIOTask(task_id, action);
  }

  io_tasks_.at(task_id).warning_info.emplace(
      std::move(warning_files), warning_reason, std::move(callback),
      base::BindOnce(
          &FilesPolicyNotificationManager::OnIOTaskWarningDialogClicked,
          weak_factory_.GetWeakPtr(), task_id, warning_reason));

  file_manager::io_task::PauseParams pause_params;
  pause_params.policy_params =
      file_manager::io_task::PolicyPauseParams(Policy::kDlp);
  // TODO(b/285880274): Pass number of files on PolicyPauseParams because it's
  // needed for the strings.
  io_task_controller->Pause(task_id, std::move(pause_params));
  // TODO(ayaelattar): Timeout after total 5 minutes.
}

void FilesPolicyNotificationManager::StartTimer(
    DialogInfo* info,
    base::OnceClosure on_timeout_callback) {
  info->timeout_timer.SetTaskRunner(task_runner_);
  info->timeout_timer.Start(FROM_HERE, kOpenFilesAppTimeout,
                            std::move(on_timeout_callback));
}

void FilesPolicyNotificationManager::OnIOTaskTimedOut(
    file_manager::io_task::IOTaskId task_id) {
  if (pending_dialogs_.empty()) {
    return;
  }
  DCHECK(pending_dialogs_.front()->task_id == task_id);
  // Stop waiting for the Files App and fallback to system modal.
  ShowPendingDialog(/*modal_parent=*/nullptr);
}

void FilesPolicyNotificationManager::OnNonIOTaskTimedOut(
    std::string notification_id) {
  // If the notification id doesn't match the front element, we already showed
  // the dialog for this notification before timing out.
  if (pending_dialogs_.empty()) {
    return;
  }
  DCHECK(pending_dialogs_.front()->notification_id == notification_id);
  // Stop waiting for the Files App and fallback to system modal.
  ShowPendingDialog(/*modal_parent=*/nullptr);
}

void FilesPolicyNotificationManager::ShowPendingDialog(
    gfx::NativeWindow modal_parent) {
  if (pending_dialogs_.empty()) {
    return;
  }
  // Pop the dialog. This also stops the timer if it hasn't fired already.
  CHECK(pending_dialogs_.front()->dialog_callback);
  std::move(pending_dialogs_.front()->dialog_callback).Run(modal_parent);
  pending_dialogs_.pop();
  // If this was the last dialog, stop observing the browser list.
  if (pending_dialogs_.empty()) {
    BrowserList::RemoveObserver(this);
  }
}

}  // namespace policy
