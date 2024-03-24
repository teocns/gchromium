#include <memory>
#include <optional>
#include <string>
#include <unordered_set>

#include "base/base_switches.h"
#include "base/lazy_instance.h"
#include "base/path_service.h"
#include "base/time/time.h"
#include "build/build_config.h"
#include "chrome/browser/chrome_content_browser_client.h"
#include "chrome/common/chrome_features.h"
#include "chrome/common/chrome_paths.h"
#include "chrome/common/chrome_paths_internal.h"
#include "chrome/common/chrome_switches.h"
#include "components/component_updater/component_updater_switches.h"
#include "components/embedder_support/switches.h"
#include "components/sync/base/command_line_switches.h"
#include "components/variations/variations_switches.h"
#include "content/public/common/content_switches.h"
#include "ghost/app/ghost_main_delegate.h"
#include "ghost/browser/ghost_content_browser_client.h"
#include "ghost/renderer/ghost_content_renderer_client.h"
#include "google_apis/gaia/gaia_switches.h"

namespace {}  // namespace

#if !defined(CHROME_MULTIPLE_DLL_BROWSER)
base::LazyInstance<GhostContentRendererClient>::DestructorAtExit
    g_ghost_content_renderer_client = LAZY_INSTANCE_INITIALIZER;
#endif

#if !defined(CHROME_MULTIPLE_DLL_CHILD)
base::LazyInstance<GhostContentBrowserClient>::DestructorAtExit
    g_ghost_content_browser_client = LAZY_INSTANCE_INITIALIZER;
#endif

GhostMainDelegate::GhostMainDelegate() : ChromeMainDelegate() {}

GhostMainDelegate::GhostMainDelegate(base::TimeTicks exe_entry_point_ticks)
    : ChromeMainDelegate(exe_entry_point_ticks) {}

GhostMainDelegate::~GhostMainDelegate() {}

content::ContentRendererClient*
GhostMainDelegate::CreateContentRendererClient() {
#if defined(CHROME_MULTIPLE_DLL_BROWSER)
  return NULL;
#else
  return g_ghost_content_renderer_client.Pointer();
#endif
}

content::ContentBrowserClient* GhostMainDelegate::CreateContentBrowserClient() {
#if defined(CHROME_MULTIPLE_DLL_CHILD)
  return NULL;
#else
  if (chrome_content_browser_client_ == nullptr) {
    chrome_content_browser_client_ =
        std::make_unique<GhostContentBrowserClient>();
  }
  return chrome_content_browser_client_.get();
#endif
}
