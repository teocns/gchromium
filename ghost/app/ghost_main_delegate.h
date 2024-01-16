#ifndef GHOST_APP_MAIN_DELEGATE_H
#define GHOST_APP_MAIN_DELEGATE_H

#include <optional>

#include "build/build_config.h"
#include "chrome/app/chrome_main_delegate.h"

// Chrome implementation of ContentMainDelegate.
class GhostMainDelegate : public ChromeMainDelegate {
 public:
  GhostMainDelegate(const GhostMainDelegate&) = delete;
  GhostMainDelegate& operator=(const GhostMainDelegate&) = delete;
  GhostMainDelegate();
  // using ChromeMainDelegate::ChromeMainDelegate;
  

  // |exe_entry_point_ticks| is the time at which the main function of the
  // executable was entered, or null if not available.
  explicit GhostMainDelegate(base::TimeTicks exe_entry_point_ticks);
  ~GhostMainDelegate() override;

 protected:
  // content::ContentMainDelegate implementation:
  content::ContentBrowserClient* CreateContentBrowserClient() override;
  content::ContentRendererClient* CreateContentRendererClient() override;
  // content::ContentUtilityClient* CreateContentUtilityClient() override;
  // void PreSandboxStartup() override;
  // std::optional<int> PostEarlyInitialization(
  //     ChromeMainDelegate::InvokedIn invoked_in) override;
};

#endif  // GHOST_APP_MAIN_DELEGATE_H
