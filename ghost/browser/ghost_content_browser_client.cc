#include "ghost/browser/ghost_content_browser_client.h"
#include <algorithm>
#include <optional>
#include <string>
#include <utility>
#include <vector>
#include "base/command_line.h"
#include "base/logging.h"
#include "chrome/browser/chrome_content_browser_client.h"

#include "content/browser/renderer_host/render_process_host_impl.h"
#include "content/public/browser/render_process_host.h"
#include "fingerprinting/public/cpp/manager.h"
#include "fingerprinting/public/mojom/manager.mojom.h"
#include "mojo/public/cpp/bindings/pending_receiver.h"
#include "mojo/public/cpp/bindings/receiver.h"
#include "mojo/public/cpp/bindings/self_owned_receiver.h"
#include "services/service_manager/public/cpp/binder_registry.h"

// #include "net/base/registry_controlled_domains/registry_controlled_domain.h"
// #include "net/cookies/site_for_cookies.h"
// #include "services/service_manager/public/cpp/binder_registry.h"
// #include

#include "third_party/blink/public/common/features.h"

// #include "third_party/blink/public/common/loader/url_loader_throttle.h"
// #include "third_party/widevine/cdm/buildflags.h"
// #include "ui/base/l10n/l10n_util.h"

// using blink::web_pref::WebPreferences;
// using content::BrowserThread;
// using content::ContentBrowserClient;
// using content::RenderFrameHost;
// using content::WebContents;

using content::BrowserThread;
using content::ContentBrowserClient;
using content::RenderFrameHost;
using content::WebContents;
using fingerprinting::FingerprintManager;
GhostContentBrowserClient::GhostContentBrowserClient()
    : ChromeContentBrowserClient() {
  LOG(INFO) << "GhostContentBrowserClient::GhostContentBrowserClient";
}

void GhostContentBrowserClient::ExposeInterfacesToRenderer(
    service_manager::BinderRegistry* registry,
    blink::AssociatedInterfaceRegistry* associated_registry,
    content::RenderProcessHost* render_process_host) {
  ChromeContentBrowserClient::ExposeInterfacesToRenderer(
      registry, associated_registry, render_process_host);

  // We can downcast the RenderProcessHost to RenderProcessHostImpl safe to do
  // so. it here since we're in the browser render_process_host

  content::RenderProcessHostImpl* render_process_host_impl =
      static_cast<content::RenderProcessHostImpl*>(render_process_host);

  render_process_host_impl->AddUIThreadInterface(
      registry,
      base::BindRepeating(
          [](mojo::PendingReceiver<fingerprinting::mojom::FingerprintManager>
                 receiver) {
            fingerprinting::FingerprintManager::GetInstance()->Bind(
                std::move(receiver));
          }));
}

// GetUserAgentMetadata
blink::UserAgentMetadata GhostContentBrowserClient::GetUserAgentMetadata() {
  // Attempt to retrieve from fingerprint, otherwise return default
  // TODO: this should be moved to extra parts, or at least not use static
  // access

  auto fp = fingerprinting::FingerprintManager::GetInstance();
  // fp->GetUserAgentMetadata();
  if (!fp->Loaded()) {
    // Fall back to superclass
    return ChromeContentBrowserClient::GetUserAgentMetadata();
  }

  return fp->GetUserAgentMetadata_().value();

  // ->GetUserAgentMetadata();
}

std::string GhostContentBrowserClient::GetUserAgent() {
  auto fp = fingerprinting::FingerprintManager::GetInstance();
  if (!fp->Loaded()) {
    // Fall back to superclass
    return ChromeContentBrowserClient::GetUserAgent();
  }

  return fp->GetUserAgent_().value();
}

std::string GhostContentBrowserClient::GetUserAgentBasedOnPolicy(content::BrowserContext* context) {
  auto fp = fingerprinting::FingerprintManager::GetInstance();
  if (!fp->Loaded()) {
    // Fall back to superclass
    return ChromeContentBrowserClient::GetUserAgentBasedOnPolicy(context);
  }

  return fp->GetUserAgent_().value();
}
// @@ -2653,6 +2654,13 @@ void RenderProcessHostImpl::RegisterMojoInterfaces() {
//                            instance_weak_factory_.GetWeakPtr()));
//  #endif
//
//   // Register Fingerprinting interfaces
//
//   AddUIThreadInterface(
//       registry.get(),
//       base::BindRepeating(&fingerprinting::FingerprintManager::Bind,
//       base::Unretained(fingerprinting::FingerprintManager::GetInstance()))
//   );
// ---- Please do not register interfaces below this line ------
//
// This call should be done after registering all interfaces above, so that
// registry->AddReceiver<FingerprintManager>(
//     base::BindRepeating(
//         &FingerprintManager::Bind,
//         base::Unretained(FingerprintManager::GetInstance())),
//     content::GetUIThreadTaskRunner({}));
//
//
//
//
//
// auto taskRunner = content::GetUIThreadTaskRunner({});
// auto ref = &fingerprinting::FingerprintManager::Bind;
// AddUIThreadInterface(
//     registry,
//   );
//

// // Helper method that checks the RenderProcessHost is still alive before
// hopping
// // over to the IO thread.
// void MaybeBindFingerprinting(
//     int rph_id,
//     content::ResourceContext* resource_context,
//     base::RepeatingCallback<scoped_refptr<safe_browsing::UrlCheckerDelegate>()>
//         get_checker_delegate,
//     mojo::PendingReceiver<safe_browsing::mojom::SafeBrowsing> receiver) {
//   DCHECK_CURRENTLY_ON(content::BrowserThread::UI);
//
//   content::RenderProcessHost* render_process_host =
//       content::RenderProcessHost::FromID(rph_id);
//   if (!render_process_host)
//     return;
//
//   content::GetIOThreadTaskRunner({})->PostTask(
//       FROM_HERE,
//       base::BindOnce(&safe_browsing::MojoSafeBrowsingImpl::MaybeCreate,
//       rph_id,
//                      resource_context, std::move(get_checker_delegate),
//                      std::move(receiver)));
// }

// Register Fingerprinting interfaces

// registry->AddInterface<fingerprinting::mojom::FingerprintManager>(
//       base::BindRepeating(
//           &media::MediaCapsImpl::AddReceiver,
//           base::Unretained(cast_browser_main_parts_->media_caps())),
//       base::SingleThreadTaskRunner::GetCurrentDefault());
//

// GetTaskRunnerForThread()
// registry->AddInterface<mojom::MemoryPressureController>(
//     base::BindRepeating(&MemoryPressureControllerImpl::AddReceiver,
//                         base::Unretained(memory_pressure_controller_.get())),
//     base::SingleThreadTaskRunner::GetCurrentDefault());

// registry->AddInterface(
//     base::BindRepeating(&Initialize, render_process_host->GetID()),
//     content::GetUIThreadTaskRunner({}));

// content::BrowserContext* context =
// render_process_host->GetBrowserContext(); mojo::MakeSelfOwnedReceiver(
//     std::make_unique<fingerprinting::mojom::BraveSearchFallbackHost>(
//         context->GetDefaultStoragePartition()
//             ->GetURLLoaderFactoryForBrowserProcess()),
//     std::move(receiver));
// }
// void GhostContentBrowserClient::ExposeInterfacesToRenderer(
//     service_manager::BinderRegistry* registry,
//     blink::AssociatedInterfaceRegistry* associated_registry,
//     content::RenderProcessHost* render_process_host) {
//   ChromeContentBrowserClient::ExposeInterfacesToRenderer(
//       registry, associated_registry, render_process_host);

// registry->AddUIThreadInterface(
//     base::BindRepeating(&Initialize),
//     base::SequencedTaskRunnerHandle::Get()
// );

// registry->AddInterface(
//     base::BindRepeating(&Initialize, render_process_host->GetID()),
//     content::GetUIThreadTaskRunner({}));
// }
// void GhostContentBrowserClient::ExposeInterfacesToRenderer(
//     service_manager::BinderRegistry* registry,
//     blink::AssociatedInterfaceRegistry* associated_registry,
//     content::RenderProcessHost* render_process_host) {
//   ChromeContentBrowserClient::ExposeInterfacesToRenderer(
//       registry, associated_registry, render_process_host);
//
//   // registry->AddInterface(base::BindRepeating(&BindFingerprintingManager,
//   // render_process_host->GetID()),
//   //                        content::GetUIThreadTaskRunner({}));
// }

//
//
//   AddUIThreadInterface(
//       registry.get(),
//       base::BindRepeating(&fingerprinting::FingerprintManager::Bind,
//       base::Unretained(fingerprinting::FingerprintManager::GetInstance()))
//   );
//
//
//

// void BindFingerprintingManager(
//     mojo::PendingReceiver<fingerprinting::mojom::FingerprintManager>
//     receiver) {
//   fingerprint_manager_.AddReceiver(std::move(receiver));
// }

// void BindBraveSearchDefaultHost(
//     content::RenderFrameHost* const frame_host,
//     mojo::PendingReceiver<brave_search::mojom::BraveSearchDefault> receiver)
//     {
//   auto* context = frame_host->GetBrowserContext();
//   auto* profile = Profile::FromBrowserContext(context);
//   if (brave::IsRegularProfile(profile)) {
//     auto* template_url_service =
//         TemplateURLServiceFactory::GetForProfile(profile);
//     const std::string host = frame_host->GetLastCommittedURL().host();
//     mojo::MakeSelfOwnedReceiver(
//         std::make_unique<brave_search::BraveSearchDefaultHost>(
//             host, template_url_service, profile->GetPrefs()),
//         std::move(receiver));
//   } else {
//     // Dummy API which always returns false for private contexts.
//     mojo::MakeSelfOwnedReceiver(
//         std::make_unique<brave_search::BraveSearchDefaultHostPrivate>(),
//         std::move(receiver));
//   }
// }
