// Copyright 2014 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import "ios/chrome/browser/ui/bookmarks/home/bookmark_promo_controller.h"

#import <memory>

#import "components/bookmarks/common/bookmark_features.h"
#import "components/prefs/pref_service.h"
#import "components/signin/public/base/signin_pref_names.h"
#import "components/signin/public/identity_manager/account_info.h"
#import "components/signin/public/identity_manager/identity_manager.h"
#import "components/signin/public/identity_manager/objc/identity_manager_observer_bridge.h"
#import "components/sync/service/sync_service.h"
#import "ios/chrome/browser/shared/model/browser/browser.h"
#import "ios/chrome/browser/shared/model/browser_state/chrome_browser_state.h"
#import "ios/chrome/browser/signin/authentication_service_factory.h"
#import "ios/chrome/browser/signin/chrome_account_manager_service_factory.h"
#import "ios/chrome/browser/signin/identity_manager_factory.h"
#import "ios/chrome/browser/sync/sync_service_factory.h"
#import "ios/chrome/browser/ui/authentication/cells/signin_promo_view_configurator.h"
#import "ios/chrome/browser/ui/authentication/cells/signin_promo_view_consumer.h"
#import "ios/chrome/browser/ui/authentication/signin_promo_view_mediator.h"
#import "ios/chrome/browser/ui/bookmarks/bookmark_utils_ios.h"

#if !defined(__has_feature) || !__has_feature(objc_arc)
#error "This file requires ARC support."
#endif

@interface BookmarkPromoController () <SigninPromoViewConsumer,
                                       IdentityManagerObserverBridgeDelegate>

@end

@implementation BookmarkPromoController {
  base::WeakPtr<Browser> _browser;
  std::unique_ptr<signin::IdentityManagerObserverBridge>
      _identityManagerObserverBridge;
  // Mediator to use for the sign-in promo view displayed in the bookmark view.
  SigninPromoViewMediator* _signinPromoViewMediator;
}

- (instancetype)initWithBrowser:(Browser*)browser
                       delegate:(id<BookmarkPromoControllerDelegate>)delegate
                      presenter:(id<SigninPresenter>)presenter
             baseViewController:(UIViewController*)baseViewController {
  DCHECK(browser);
  self = [super init];
  if (self) {
    _delegate = delegate;
    ChromeBrowserState* browserState =
        browser->GetBrowserState()->GetOriginalChromeBrowserState();
    _browser = browser->AsWeakPtr();
    _identityManagerObserverBridge.reset(
        new signin::IdentityManagerObserverBridge(
            IdentityManagerFactory::GetForBrowserState(browserState), self));
    _signinPromoViewMediator = [[SigninPromoViewMediator alloc]
              initWithBrowser:browser
        accountManagerService:ChromeAccountManagerServiceFactory::
                                  GetForBrowserState(browserState)
                  authService:AuthenticationServiceFactory::GetForBrowserState(
                                  browserState)
                  prefService:browserState->GetPrefs()
                  accessPoint:signin_metrics::AccessPoint::
                                  ACCESS_POINT_BOOKMARK_MANAGER
                    presenter:presenter
           baseViewController:baseViewController];
    _signinPromoViewMediator.consumer = self;
    [self updateShouldShowSigninPromo];
  }
  return self;
}

- (void)dealloc {
  CHECK(!_browser);
}

- (void)shutdown {
  [_signinPromoViewMediator disconnect];
  _signinPromoViewMediator = nil;
  _browser = nullptr;
  _identityManagerObserverBridge.reset();
}

- (void)hidePromoCell {
  DCHECK(_browser);
  self.shouldShowSigninPromo = NO;
}

- (void)setShouldShowSigninPromo:(BOOL)shouldShowSigninPromo {
  if (_shouldShowSigninPromo != shouldShowSigninPromo) {
    _shouldShowSigninPromo = shouldShowSigninPromo;
    [self.delegate promoStateChanged:shouldShowSigninPromo];
  }
}

- (void)updateShouldShowSigninPromo {
  DCHECK(_browser);
  ChromeBrowserState* browserState =
      _browser->GetBrowserState()->GetOriginalChromeBrowserState();
  AuthenticationService* authenticationService =
      AuthenticationServiceFactory::GetForBrowserState(browserState);
  if (![SigninPromoViewMediator
          shouldDisplaySigninPromoViewWithAccessPoint:
              signin_metrics::AccessPoint::ACCESS_POINT_BOOKMARK_MANAGER
                                authenticationService:authenticationService
                                          prefService:browserState
                                                          ->GetPrefs()]) {
    self.shouldShowSigninPromo = NO;
    return;
  }
  signin::IdentityManager* identityManager =
      IdentityManagerFactory::GetForBrowserState(browserState);
  if (!identityManager->HasPrimaryAccount(signin::ConsentLevel::kSignin)) {
    if (base::FeatureList::IsEnabled(
            bookmarks::kEnableBookmarksAccountStorage)) {
      PrefService* prefs = browserState->GetPrefs();
      const std::string lastSignedInGaiaId =
          prefs->GetString(prefs::kGoogleServicesLastGaiaId);
      // If the last signed-in user did not remove data during sign-out, don't
      // show the signin promo.
      if (lastSignedInGaiaId.empty()) {
        self.shouldShowSigninPromo = YES;
        _signinPromoViewMediator.signInOnly = YES;
      } else {
        self.shouldShowSigninPromo = NO;
      }
    } else {
      // If the user is not signed in, the promo should be visible.
      self.shouldShowSigninPromo = YES;
      _signinPromoViewMediator.signInOnly = NO;
    }
    return;
  }
  if (identityManager->HasPrimaryAccount(signin::ConsentLevel::kSync)) {
    // If the user is already syncing, the promo should not be visible.
    self.shouldShowSigninPromo = NO;
    return;
  }
  syncer::SyncService* syncService =
      SyncServiceFactory::GetForBrowserState(browserState);
  if (!bookmark_utils_ios::IsAccountBookmarkStorageOptedIn(syncService)) {
    // The user signed in, but not opted into syncing bookmarks - show sync
    // promo.
    self.shouldShowSigninPromo = YES;
    _signinPromoViewMediator.signInOnly = NO;
    return;
  }
  if ([self.delegate isPerformingInitialSync]) {
    // The user is opted into syncing bookmarks, but the first sync is not
    // finished yet - keep the promo visible to show the spinner.
    self.shouldShowSigninPromo = YES;
    _signinPromoViewMediator.signInOnly = YES;
    return;
  }
  // The user is opted into syncing bookmarks and the first sync is done - hide
  // the promo.
  self.shouldShowSigninPromo = NO;
}

#pragma mark - IdentityManagerObserverBridgeDelegate

// Called when a user changes the syncing state.
- (void)onPrimaryAccountChanged:
    (const signin::PrimaryAccountChangeEvent&)event {
  if (base::FeatureList::IsEnabled(bookmarks::kEnableBookmarksAccountStorage)) {
    // The account storage promo is not shown if the user is signed-in, so
    // events with sign-in consent level should be captured and handled.
    [self handlePrimaryAccountChange:event
                        consentLevel:signin::ConsentLevel::kSignin];
  } else {
    [self handlePrimaryAccountChange:event
                        consentLevel:signin::ConsentLevel::kSync];
  }
}

#pragma mark - SigninPromoViewConsumer

- (void)configureSigninPromoWithConfigurator:
            (SigninPromoViewConfigurator*)configurator
                             identityChanged:(BOOL)identityChanged {
  [self.delegate configureSigninPromoWithConfigurator:configurator
                                      identityChanged:identityChanged];
}

- (void)signinDidFinish {
  [self updateShouldShowSigninPromo];
}

- (void)signinPromoViewMediatorCloseButtonWasTapped:
    (SigninPromoViewMediator*)mediator {
  [self updateShouldShowSigninPromo];
}

#pragma mark - Private methods

// Handles the given primary account change event for the given consent level.
- (void)handlePrimaryAccountChange:
            (const signin::PrimaryAccountChangeEvent&)event
                      consentLevel:(signin::ConsentLevel)consentLevel {
  switch (event.GetEventTypeFor(consentLevel)) {
    case signin::PrimaryAccountChangeEvent::Type::kSet:
      if (!self.signinPromoViewMediator.signinInProgress) {
        self.shouldShowSigninPromo = NO;
      }
      break;
    case signin::PrimaryAccountChangeEvent::Type::kCleared:
      [self updateShouldShowSigninPromo];
      break;
    case signin::PrimaryAccountChangeEvent::Type::kNone:
      break;
  }
}

@end
