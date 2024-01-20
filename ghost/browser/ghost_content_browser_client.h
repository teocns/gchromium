#ifndef GHOST_BROWSER_GHOST_CONTENT_BROWSER_CLIENT_H_
#define GHOST_BROWSER_GHOST_CONTENT_BROWSER_CLIENT_H_

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "chrome/browser/chrome_content_browser_client.h"
#include "content/public/browser/browser_thread.h"
#include "content/public/browser/content_browser_client.h"
class PrefChangeRegistrar;

namespace content {
class BrowserContext;
class RenderProcessHost;
}  // namespace content

namespace blink {
class AssociatedInterfaceRegistry;
}  // namespace blink
// namespace web_pref {
// struct WebPreferences;
// }  // namespace web_pref

class GhostContentBrowserClient : public ChromeContentBrowserClient {
 public:
  GhostContentBrowserClient();
  // GhostContentBrowserClient(const GhostContentBrowserClient&) = delete;
  // GhostContentBrowserClient& operator=(const GhostContentBrowserClient&) =
  //     delete;
  // ~GhostContentBrowserClient() override = default;

  void ExposeInterfacesToRenderer(
      service_manager::BinderRegistry* registry,
      blink::AssociatedInterfaceRegistry* associated_registry,
      content::RenderProcessHost* render_process_host) override;

  // blink::UserAgentMetadata GetUserAgentMetadata() override;
  // std::string GetProduct() override;
  // std::string GetUserAgent() override;
  // std::string GetUserAgentBasedOnPolicy(
  //     content::BrowserContext* context) override;

  // ~GhostContentBrowserClient() override;
  // std::unique_ptr<MemoryPressureControllerImpl> memory_pressure_controller_;
};

#endif  // GHOST_BROWSER_GHOST_CONTENT_BROWSER_CLIENT_H_
