#include <memory>
#include <optional>
#include <string>
#include <unordered_set>

#include "base/base_switches.h"
#include "base/lazy_instance.h"
#include "base/path_service.h"
#include "base/time/time.h"
// #include "brave/app/brave_command_line_helper.h"
// #include "brave/browser/brave_content_browser_client.h"
// #include "brave/common/resource_bundle_helper.h"
// #include "brave/components/brave_component_updater/browser/features.h"
// #include "brave/components/brave_component_updater/browser/switches.h"
// #include "brave/components/constants/brave_switches.h"
// #include "brave/components/speedreader/common/buildflags/buildflags.h"
// #include "brave/components/update_client/buildflags.h"
#include "ghost/app/ghost_main_delegate.h"
#include "ghost/renderer/ghost_content_renderer_client.h"
// #include "brave/utility/brave_content_utility_client.h"
#include "build/build_config.h"
#include "chrome/common/chrome_features.h"
#include "chrome/common/chrome_paths.h"
#include "chrome/common/chrome_paths_internal.h"
#include "chrome/common/chrome_switches.h"
#include "components/component_updater/component_updater_switches.h"
#include "components/embedder_support/switches.h"
#include "components/sync/base/command_line_switches.h"
#include "components/variations/variations_switches.h"
#include "content/public/common/content_switches.h"
#include "ghost/renderer/ghost_content_renderer_client.h"
#include "google_apis/gaia/gaia_switches.h"


namespace {}  // namespace

base::LazyInstance<GhostContentRenderClient>::DestructorAtExit
    g_ghost_content_renderer_client = LAZY_INSTANCE_INITIALIZER;

content::ContentRendererClient*
GhostMainDelegate::CreateContentRendererClient() {
  return g_ghost_content_renderer_client.Pointer();
}
