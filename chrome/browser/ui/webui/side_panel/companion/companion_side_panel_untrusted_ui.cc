// Copyright 2023 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/browser/ui/webui/side_panel/companion/companion_side_panel_untrusted_ui.h"

#include "chrome/browser/companion/core/utils.h"
#include "chrome/browser/media/webrtc/media_capture_devices_dispatcher.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/side_panel/companion/companion_side_panel_controller_utils.h"
#include "chrome/browser/ui/webui/side_panel/companion/companion_page_handler.h"
#include "chrome/common/webui_url_constants.h"
#include "chrome/grit/side_panel_companion_resources.h"
#include "chrome/grit/side_panel_companion_resources_map.h"
#include "content/public/browser/web_contents.h"
#include "content/public/browser/web_ui.h"
#include "content/public/browser/web_ui_data_source.h"
#include "url/gurl.h"

CompanionSidePanelUntrustedUI::CompanionSidePanelUntrustedUI(
    content::WebUI* web_ui)
    : ui::UntrustedBubbleWebUIController(web_ui) {
  // Set up the chrome-untrusted://companion-side-panel source.
  content::WebUIDataSource* html_source =
      content::WebUIDataSource::CreateAndAdd(
          web_ui->GetWebContents()->GetBrowserContext(),
          chrome::kChromeUIUntrustedCompanionSidePanelURL);

  // Add required resources.
  html_source->UseStringsJs();
  html_source->AddResourcePaths(base::make_span(
      kSidePanelCompanionResources, kSidePanelCompanionResourcesSize));
  html_source->AddResourcePath("", IDR_SIDE_PANEL_COMPANION_COMPANION_HTML);
  // Allow untrusted mojo resources to be loaded.
  html_source->OverrideContentSecurityPolicy(
      network::mojom::CSPDirectiveName::ScriptSrc,
      "script-src chrome-untrusted://resources 'self';");
  // Allow the companion homepage URL to be embedded in this WebUI.
  GURL frameSrcUrl =
      GURL(companion::GetHomepageURLForCompanion()).GetWithEmptyPath();
  std::string frameSrcString = frameSrcUrl.is_valid()
                                   ? frameSrcUrl.spec()
                                   : companion::GetHomepageURLForCompanion();
  // Allow iframing accounts page due to potential redirects.
  std::string frameSrcDirective =
      std::string("frame-src https://accounts.google.com ") + frameSrcString +
      ";";
  std::string formActionDirective =
      std::string("form-action ") + frameSrcString + ";";
  html_source->OverrideContentSecurityPolicy(
      network::mojom::CSPDirectiveName::FrameSrc, frameSrcDirective);
  html_source->OverrideContentSecurityPolicy(
      network::mojom::CSPDirectiveName::FormAction, formActionDirective);
  html_source->AddString("companion_origin", frameSrcString);

  web_ui->GetWebContents()->SetDelegate(this);
}

CompanionSidePanelUntrustedUI::~CompanionSidePanelUntrustedUI() = default;

void CompanionSidePanelUntrustedUI::BindInterface(
    mojo::PendingReceiver<side_panel::mojom::CompanionPageHandlerFactory>
        receiver) {
  companion_page_factory_receiver_.reset();
  companion_page_factory_receiver_.Bind(std::move(receiver));
}

void CompanionSidePanelUntrustedUI::CreateCompanionPageHandler(
    mojo::PendingReceiver<side_panel::mojom::CompanionPageHandler> receiver,
    mojo::PendingRemote<side_panel::mojom::CompanionPage> page) {
  companion_page_handler_ = std::make_unique<companion::CompanionPageHandler>(
      std::move(receiver), std::move(page), this);
}

void CompanionSidePanelUntrustedUI::RequestMediaAccessPermission(
    content::WebContents* web_contents,
    const content::MediaStreamRequest& request,
    content::MediaResponseCallback callback) {
  // Note: This is needed for taking screenshots via the feedback form.
  MediaCaptureDevicesDispatcher::GetInstance()->ProcessMediaAccessRequest(
      web_contents, request, std::move(callback), /*extension=*/nullptr);
}

base::WeakPtr<CompanionSidePanelUntrustedUI>
CompanionSidePanelUntrustedUI::GetWeakPtr() {
  return weak_factory_.GetWeakPtr();
}

content::WebContents* CompanionSidePanelUntrustedUI::OpenURLFromTab(
    content::WebContents* source,
    const content::OpenURLParams& params) {
  auto* browser = companion::GetBrowserForWebContents(source);
  if (browser) {
    browser->OpenURL(params);
  }
  return nullptr;
}

CompanionSidePanelUntrustedUIConfig::CompanionSidePanelUntrustedUIConfig()
    : WebUIConfig(content::kChromeUIUntrustedScheme,
                  chrome::kChromeUIUntrustedCompanionSidePanelHost) {}

std::unique_ptr<content::WebUIController>
CompanionSidePanelUntrustedUIConfig::CreateWebUIController(
    content::WebUI* web_ui,
    const GURL& url) {
  return std::make_unique<CompanionSidePanelUntrustedUI>(web_ui);
}

WEB_UI_CONTROLLER_TYPE_IMPL(CompanionSidePanelUntrustedUI)
