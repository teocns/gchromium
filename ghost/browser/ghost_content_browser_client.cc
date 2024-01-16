#include <algorithm>
#include <optional>
#include <string>
#include <utility>
#include <vector>
//
#include "base/command_line.h"
#include "base/logging.h"
// #include "base/functional/bind.h"
// #include "base/json/json_reader.h"
// #include "base/strings/strcat.h"
// #include "base/system/sys_info.h"
// #include "build/build_config.h"
// #include "chrome/browser/browser_process.h"
// #include "chrome/browser/chrome_browser_interface_binders.h"
// #include "chrome/browser/chrome_content_browser_client.h"
// #include
// "chrome/browser/content_settings/host_content_settings_map_factory.h"
// #include
// "components/content_settings/core/browser/host_content_settings_map.h"
// #include "components/embedder_support/switches.h"
// #include "components/prefs/pref_service.h"
// #include "components/version_info/version_info.h"
// #include "content/public/browser/browser_context.h"
// #include "content/public/browser/browser_task_traits.h"
// #include "content/public/browser/browser_thread.h"
// #include "content/public/browser/browser_url_handler.h"
// #include "content/public/browser/navigation_handle.h"
// #include "content/public/browser/render_frame_host.h"
// #include "content/public/browser/render_process_host.h"
// #include "content/public/browser/storage_partition.h"
// #include "content/public/browser/weak_document_ptr.h"
// #include "content/public/browser/web_ui_browser_interface_broker_registry.h"
// #include "content/public/browser/web_ui_controller_interface_binder.h"
// #include "content/public/common/content_client.h"
// #include "content/public/common/content_switches.h"
// #include "content/public/common/url_constants.h"
// #include "extensions/buildflags/buildflags.h"
#include "ghost/browser/ghost_content_browser_client.h"
// #include "mojo/public/cpp/bindings/pending_receiver.h"
// #include "mojo/public/cpp/bindings/self_owned_receiver.h"
// #include "net/base/registry_controlled_domains/registry_controlled_domain.h"
// #include "net/cookies/site_for_cookies.h"
// #include "services/service_manager/public/cpp/binder_registry.h"
// #include
// "third_party/blink/public/common/associated_interfaces/associated_interface_registry.h"
// #include "third_party/blink/public/common/features.h"
// #include "third_party/blink/public/common/loader/url_loader_throttle.h"
// #include
// "third_party/blink/public/mojom/webpreferences/web_preferences.mojom.h"
// #include "third_party/widevine/cdm/buildflags.h"
// #include "ui/base/l10n/l10n_util.h"

// using blink::web_pref::WebPreferences;
// using content::BrowserThread;
// using content::ContentBrowserClient;
// using content::RenderFrameHost;
// using content::WebContents;

GhostContentBrowserClient::GhostContentBrowserClient() {
  LOG(INFO) << "GhostContentBrowserClient::GhostContentBrowserClient";
}
