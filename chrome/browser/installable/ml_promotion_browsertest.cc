// Copyright 2023 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <memory>
#include <vector>

#include "base/memory/scoped_refptr.h"
#include "base/run_loop.h"
#include "base/test/bind.h"
#include "base/test/gmock_callback_support.h"
#include "base/test/scoped_feature_list.h"
#include "base/test/test_future.h"
#include "base/test/test_simple_task_runner.h"
#include "chrome/browser/banners/app_banner_manager_desktop.h"
#include "chrome/browser/banners/test_app_banner_manager_desktop.h"
#include "chrome/browser/installable/ml_promotion_browsertest_base.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/browser_commands.h"
#include "chrome/browser/ui/web_applications/test/web_app_browsertest_util.h"
#include "chrome/browser/web_applications/web_app_command_manager.h"
#include "chrome/browser/web_applications/web_app_provider.h"
#include "components/segmentation_platform/public/constants.h"
#include "components/segmentation_platform/public/input_context.h"
#include "components/segmentation_platform/public/testing/mock_segmentation_platform_service.h"
#include "components/segmentation_platform/public/trigger.h"
#include "components/segmentation_platform/public/types/processed_value.h"
#include "components/ukm/test_ukm_recorder.h"
#include "components/webapps/browser/features.h"
#include "components/webapps/browser/install_result_code.h"
#include "components/webapps/browser/installable/installable_metrics.h"
#include "components/webapps/browser/installable/metrics/site_quality_metrics_task.h"
#include "components/webapps/browser/installable/ml_install_operation_tracker.h"
#include "components/webapps/browser/installable/ml_install_result_reporter.h"
#include "components/webapps/browser/installable/ml_installability_promoter.h"
#include "content/public/browser/service_worker_context.h"
#include "content/public/browser/service_worker_context_observer.h"
#include "content/public/browser/web_contents.h"
#include "content/public/browser/web_contents_observer.h"
#include "content/public/test/browser_test.h"
#include "net/test/embedded_test_server/embedded_test_server.h"
#include "services/metrics/public/cpp/ukm_builders.h"
#include "testing/gmock/include/gmock/gmock.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "third_party/blink/public/common/manifest/manifest_util.h"
#include "third_party/blink/public/mojom/favicon/favicon_url.mojom.h"
#include "third_party/blink/public/mojom/manifest/manifest.mojom.h"
#include "ui/gfx/geometry/size.h"
#include "ui/views/test/dialog_test.h"
#include "ui/views/test/widget_test.h"
#include "ui/views/widget/any_widget_observer.h"
#include "ui/views/widget/widget.h"
#include "url/gurl.h"
#include "url/origin.h"
#include "url/url_constants.h"

namespace webapps {
namespace {
using InstallUkmEntry = ukm::builders::Site_Install;
using ManifestUkmEntry = ukm::builders::Site_Manifest;
using QualityUkmEntry = ukm::builders::Site_Quality;
using segmentation_platform::HasTrainingLabel;
using segmentation_platform::InputContext;
using segmentation_platform::IsInputContextWithArgs;
using segmentation_platform::MockSegmentationPlatformService;
using segmentation_platform::TrainingRequestId;
using segmentation_platform::processing::ProcessedValue;
using testing::_;
using testing::Pointee;
using webapps::MLInstallabilityPromoter;
using webapps::SiteInstallMetrics;
using webapps::SiteQualityMetrics;
using MlInstallResponse = MlInstallResultReporter::MlInstallResponse;

segmentation_platform::ClassificationResult CreateClassificationResult(
    std::string label,
    TrainingRequestId request_id) {
  segmentation_platform::ClassificationResult result(
      segmentation_platform::PredictionStatus::kSucceeded);
  result.ordered_labels.emplace_back(label);
  result.request_id = request_id;
  return result;
}

class ServiceWorkerLoadAwaiter : public content::ServiceWorkerContextObserver {
 public:
  ServiceWorkerLoadAwaiter(content::WebContents* web_contents, const GURL& url)
      : site_url_(url) {
    CHECK(web_contents);
    context_ = web_contents->GetPrimaryMainFrame()
                   ->GetStoragePartition()
                   ->GetServiceWorkerContext();
    context_->AddObserver(this);
  }

  ~ServiceWorkerLoadAwaiter() override {
    if (context_) {
      context_->RemoveObserver(this);
    }
  }

  bool AwaitRegistration() {
    run_loop_.Run();
    return service_worker_reg_complete_;
  }

 private:
  void OnRegistrationStored(int64_t registration_id,
                            const GURL& pattern) override {
    if (content::ServiceWorkerContext::ScopeMatches(pattern, site_url_)) {
      service_worker_reg_complete_ = true;
      run_loop_.Quit();
    }
  }

  void OnDestruct(content::ServiceWorkerContext* context) override {
    if (context_) {
      context_->RemoveObserver(this);
      context_ = nullptr;
    }
  }

  const GURL site_url_;
  raw_ptr<content::ServiceWorkerContext> context_;
  bool service_worker_reg_complete_ = false;
  base::RunLoop run_loop_;
};

class WebContentsObserverAdapter : public content::WebContentsObserver {
 public:
  explicit WebContentsObserverAdapter(content::WebContents* web_contents)
      : content::WebContentsObserver(web_contents) {}

  ~WebContentsObserverAdapter() override { Observe(nullptr); }

  bool AwaitManifestUrlChanged(GURL expected_manifest_url) {
    expected_manifest_url_ = expected_manifest_url;
    manifest_run_loop_.Run();
    return manifest_url_updated_;
  }

 private:
  void DidUpdateWebManifestURL(content::RenderFrameHost* rfh,
                               const GURL& manifest_url) override {
    if (expected_manifest_url_ == manifest_url) {
      manifest_url_updated_ = true;
      manifest_run_loop_.Quit();
    }
  }

  bool manifest_url_updated_ = true;

  GURL expected_manifest_url_;
  base::RunLoop manifest_run_loop_;
};

class MLPromotionBrowsertest : public MLPromotionBrowserTestBase {
 public:
  MLPromotionBrowsertest() {
    task_runner_ = base::MakeRefCounted<base::TestSimpleTaskRunner>();
    scoped_feature_list_.InitAndEnableFeature(
        webapps::features::kWebAppsEnableMLModelForPromotion);
  }
  ~MLPromotionBrowsertest() override = default;

  void SetUpOnMainThread() override {
    MLPromotionBrowserTestBase::SetUpOnMainThread();
    ml_promoter()->SetTaskRunnerForTesting(task_runner_);
    test_ukm_recorder_ = std::make_unique<ukm::TestAutoSetUkmRecorder>();
  }

 protected:
  GURL GetInstallableAppURL() {
    return https_server()->GetURL("/banners/manifest_test_page.html");
  }

  GURL GetUrlWithNoManifest() {
    return https_server()->GetURL("/banners/no_manifest_test_page.html");
  }

  GURL GetManifestUrlForNoManifestTestPage() {
    return https_server()->GetURL(
        "/banners/manifest_for_no_manifest_page.json");
  }

  GURL GetUrlWithManifestAllFieldsLoadedForML() {
    return https_server()->GetURL("/banners/test_page_for_ml_promotion.html");
  }

  GURL GetUrlWithNoSWNoFetchHandler() {
    return https_server()->GetURL("/banners/manifest_no_service_worker.html");
  }

  GURL GetUrlWithSWEmptyFetchHandler() {
    return https_server()->GetURL(
        "/banners/manifest_test_page_empty_fetch_handler.html");
  }

  GURL GetUrlWithSwNoFetchHandler() {
    return https_server()->GetURL(
        "/banners/no_sw_fetch_handler_test_page.html");
  }

  MLInstallabilityPromoter* ml_promoter() {
    return MLInstallabilityPromoter::FromWebContents(web_contents());
  }

  web_app::WebAppProvider& provider() {
    // TODO(b/287255120) : Block this on Android.
    return *web_app::WebAppProvider::GetForTest(profile());
  }

  const ukm::TestAutoSetUkmRecorder& test_ukm_recorder() {
    return *test_ukm_recorder_;
  }

  // Wait for service workers to be registered and the MLInstallabilityPromoter
  // to finish.
  void AwaitServiceWorkerRegistrationAndPendingDelayedTask(const GURL& url) {
    base::test::TestFuture<void> timeout_task_future;
    ml_promoter()->SetAwaitTimeoutTaskPendingCallbackForTesting(
        timeout_task_future.GetCallback());
    ServiceWorkerLoadAwaiter service_worker_loader(web_contents(), url);
    EXPECT_TRUE(service_worker_loader.AwaitRegistration());
    EXPECT_TRUE(timeout_task_future.Wait());
  }

  void AwaitManifestUrlUpdatedAndPendingDelayedTask(
      const GURL& new_manifest_url) {
    base::test::TestFuture<void> timeout_task_future;
    ml_promoter()->SetAwaitTimeoutTaskPendingCallbackForTesting(
        timeout_task_future.GetCallback());
    WebContentsObserverAdapter web_contents_observer(web_contents());
    EXPECT_TRUE(
        web_contents_observer.AwaitManifestUrlChanged(new_manifest_url));
    EXPECT_TRUE(timeout_task_future.Wait());
  }

  void NavigateAndAwaitMetricsCollectionPending(const GURL& url) {
    base::test::TestFuture<void> delayed_task_future;
    ml_promoter()->SetAwaitTimeoutTaskPendingCallbackForTesting(
        delayed_task_future.GetCallback());
    NavigateAndAwaitInstallabilityCheck(url);
    EXPECT_TRUE(delayed_task_future.Wait());
  }

  void ExpectClasificationCallReturnResult(
      GURL site_url,
      web_app::ManifestId manifest_id,
      std::string label_result,
      TrainingRequestId request_result,
      content::WebContents* custom_web_contents = nullptr) {
    if (!custom_web_contents) {
      custom_web_contents = web_contents();
    }
    base::flat_map<std::string, ProcessedValue> expected_input = {
        {"origin", ProcessedValue(url::Origin::Create(site_url).GetURL())},
        {"site_url", ProcessedValue(site_url)},
        {"manifest_id", ProcessedValue(manifest_id)}};
    EXPECT_CALL(*GetMockSegmentation(custom_web_contents),
                GetClassificationResult(
                    segmentation_platform::kWebAppInstallationPromoKey, _,
                    Pointee(testing::Field(&InputContext::metadata_args,
                                           testing::Eq(expected_input))),
                    _))
        .WillOnce(base::test::RunOnceCallback<3>(
            CreateClassificationResult(label_result, request_result)));
  }

  void ExpectTrainingResult(
      TrainingRequestId request,
      MlInstallResponse response,
      content::WebContents* custom_web_contents = nullptr) {
    if (!custom_web_contents) {
      custom_web_contents = web_contents();
    }
    EXPECT_CALL(*GetMockSegmentation(custom_web_contents),
                CollectTrainingData(
                    segmentation_platform::proto::SegmentId::
                        OPTIMIZATION_TARGET_WEB_APP_INSTALLATION_PROMO,
                    request,
                    HasTrainingLabel(
                        "WebApps.MlInstall.DialogResponse",
                        static_cast<base::HistogramBase::Sample>(response)),
                    _));
  }

  scoped_refptr<base::TestSimpleTaskRunner> task_runner_;

 private:
  base::test::ScopedFeatureList scoped_feature_list_;
  std::unique_ptr<ukm::TestAutoSetUkmRecorder> test_ukm_recorder_;
};

// Manifest Data Fetching tests.
IN_PROC_BROWSER_TEST_F(MLPromotionBrowsertest, CompletelyFilledManifestUKM) {
  NavigateAndAwaitMetricsCollectionPending(
      GetUrlWithManifestAllFieldsLoadedForML());
  task_runner_->RunPendingTasks();

  auto entries =
      test_ukm_recorder().GetEntriesByName(ManifestUkmEntry::kEntryName);
  ASSERT_EQ(entries.size(), 1u);

  auto* entry = entries[0];
  test_ukm_recorder().ExpectEntrySourceHasUrl(
      entry, GetUrlWithManifestAllFieldsLoadedForML());
  ukm::TestAutoSetUkmRecorder::ExpectEntryMetric(
      entry, ManifestUkmEntry::kDisplayModeName, /*browser=*/1);
  ukm::TestAutoSetUkmRecorder::ExpectEntryMetric(
      entry, ManifestUkmEntry::kHasBackgroundColorName, true);
  ukm::TestAutoSetUkmRecorder::ExpectEntryMetric(
      entry, ManifestUkmEntry::kHasNameName, true);
  ukm::TestAutoSetUkmRecorder::ExpectEntryMetric(
      entry, ManifestUkmEntry::kHasIconsAnyName, true);
  ukm::TestAutoSetUkmRecorder::ExpectEntryMetric(
      entry, ManifestUkmEntry::kHasIconsMaskableName, true);
  ukm::TestAutoSetUkmRecorder::ExpectEntryMetric(
      entry, ManifestUkmEntry::kHasScreenshotsName, true);
  ukm::TestAutoSetUkmRecorder::ExpectEntryMetric(
      entry, ManifestUkmEntry::kHasThemeColorName, true);
  ukm::TestAutoSetUkmRecorder::ExpectEntryMetric(
      entry, ManifestUkmEntry::kHasStartUrlName, /*kValid=*/2);
}

IN_PROC_BROWSER_TEST_F(MLPromotionBrowsertest, PartiallyFilledManifestUKM) {
  NavigateAndAwaitMetricsCollectionPending(GetInstallableAppURL());
  task_runner_->RunPendingTasks();

  auto entries =
      test_ukm_recorder().GetEntriesByName(ManifestUkmEntry::kEntryName);
  ASSERT_EQ(entries.size(), 1u);

  auto* entry = entries[0];
  test_ukm_recorder().ExpectEntrySourceHasUrl(entry, GetInstallableAppURL());
  ukm::TestAutoSetUkmRecorder::ExpectEntryMetric(
      entry, ManifestUkmEntry::kDisplayModeName, /*standalone=*/3);
  ukm::TestAutoSetUkmRecorder::ExpectEntryMetric(
      entry, ManifestUkmEntry::kHasBackgroundColorName, false);
  ukm::TestAutoSetUkmRecorder::ExpectEntryMetric(
      entry, ManifestUkmEntry::kHasNameName, true);
  ukm::TestAutoSetUkmRecorder::ExpectEntryMetric(
      entry, ManifestUkmEntry::kHasIconsAnyName, true);
  ukm::TestAutoSetUkmRecorder::ExpectEntryMetric(
      entry, ManifestUkmEntry::kHasIconsMaskableName, false);
  ukm::TestAutoSetUkmRecorder::ExpectEntryMetric(
      entry, ManifestUkmEntry::kHasScreenshotsName, false);
  ukm::TestAutoSetUkmRecorder::ExpectEntryMetric(
      entry, ManifestUkmEntry::kHasThemeColorName, false);
  ukm::TestAutoSetUkmRecorder::ExpectEntryMetric(
      entry, ManifestUkmEntry::kHasStartUrlName, /*kValid=*/2);
}

IN_PROC_BROWSER_TEST_F(MLPromotionBrowsertest, NoManifestUKM) {
  NavigateAndAwaitMetricsCollectionPending(GetUrlWithNoManifest());
  task_runner_->RunPendingTasks();

  auto entries =
      test_ukm_recorder().GetEntriesByName(ManifestUkmEntry::kEntryName);
  ASSERT_EQ(entries.size(), 1u);

  auto* entry = entries[0];
  test_ukm_recorder().ExpectEntrySourceHasUrl(entry, GetUrlWithNoManifest());
  ukm::TestAutoSetUkmRecorder::ExpectEntryMetric(
      entry, ManifestUkmEntry::kDisplayModeName, -1);
  ukm::TestAutoSetUkmRecorder::ExpectEntryMetric(
      entry, ManifestUkmEntry::kHasBackgroundColorName,
      /*NullableBoolean::Null=*/2);
  ukm::TestAutoSetUkmRecorder::ExpectEntryMetric(entry,
                                                 ManifestUkmEntry::kHasNameName,
                                                 /*NullableBoolean::Null=*/2);
  ukm::TestAutoSetUkmRecorder::ExpectEntryMetric(
      entry, ManifestUkmEntry::kHasIconsAnyName,
      /*NullableBoolean::Null=*/2);
  ukm::TestAutoSetUkmRecorder::ExpectEntryMetric(
      entry, ManifestUkmEntry::kHasIconsMaskableName,
      /*NullableBoolean::Null=*/2);
  ukm::TestAutoSetUkmRecorder::ExpectEntryMetric(
      entry, ManifestUkmEntry::kHasScreenshotsName,
      /*NullableBoolean::Null=*/2);
  ukm::TestAutoSetUkmRecorder::ExpectEntryMetric(
      entry, ManifestUkmEntry::kHasThemeColorName,
      /*NullableBoolean::Null=*/2);
  ukm::TestAutoSetUkmRecorder::ExpectEntryMetric(
      entry, ManifestUkmEntry::kHasStartUrlName, -1);
}

IN_PROC_BROWSER_TEST_F(MLPromotionBrowsertest, ManifestUpdateChangesUKM) {
  // Run the pipeline with the first update, verify no manifest data is logged
  // to UKMs.
  NavigateAndAwaitMetricsCollectionPending(GetUrlWithNoManifest());
  task_runner_->RunPendingTasks();

  auto entries =
      test_ukm_recorder().GetEntriesByName(ManifestUkmEntry::kEntryName);
  ASSERT_EQ(entries.size(), 1u);

  auto* entry = entries[0];

  // Verify UKM records empty manifest data.
  test_ukm_recorder().ExpectEntrySourceHasUrl(entry, GetUrlWithNoManifest());
  ukm::TestAutoSetUkmRecorder::ExpectEntryMetric(
      entry, ManifestUkmEntry::kDisplayModeName, -1);
  ukm::TestAutoSetUkmRecorder::ExpectEntryMetric(entry,
                                                 ManifestUkmEntry::kHasNameName,
                                                 /*NullableBoolean::Null=*/2);
  ukm::TestAutoSetUkmRecorder::ExpectEntryMetric(
      entry, ManifestUkmEntry::kHasStartUrlName, -1);

  // Restart the pipeline by simulating a refresh of the page.
  NavigateAndAwaitMetricsCollectionPending(GetUrlWithNoManifest());
  EXPECT_TRUE(content::ExecJs(
      web_contents(),
      "addManifestLinkTag('/banners/manifest_for_no_manifest_page.json')"));

  AwaitManifestUrlUpdatedAndPendingDelayedTask(
      GetManifestUrlForNoManifestTestPage());
  task_runner_->RunPendingTasks();

  auto updated_entries =
      test_ukm_recorder().GetEntriesByName(ManifestUkmEntry::kEntryName);
  ASSERT_EQ(updated_entries.size(), 2u);
  auto* updated_entry = updated_entries[1];
  test_ukm_recorder().ExpectEntrySourceHasUrl(updated_entry,
                                              GetUrlWithNoManifest());
  ukm::TestAutoSetUkmRecorder::ExpectEntryMetric(
      updated_entry, ManifestUkmEntry::kDisplayModeName, /*kStandAlone=*/3);
  ukm::TestAutoSetUkmRecorder::ExpectEntryMetric(
      updated_entry, ManifestUkmEntry::kHasNameName, true);
  ukm::TestAutoSetUkmRecorder::ExpectEntryMetric(
      updated_entry, ManifestUkmEntry::kHasStartUrlName, /*kValid=*/2);
}

// SiteInstallMetrics tests.
IN_PROC_BROWSER_TEST_F(MLPromotionBrowsertest, FullyInstalledAppMeasurement) {
  NavigateAndAwaitInstallabilityCheck(GetInstallableAppURL());
  EXPECT_TRUE(InstallAppForCurrentWebContents(/*install_locally=*/true));

  NavigateAndAwaitInstallabilityCheck(GetUrlWithNoManifest());

  // A re-navigation should retrigger the ML pipeline.
  NavigateAndAwaitMetricsCollectionPending(GetInstallableAppURL());
  task_runner_->RunPendingTasks();

  auto entries =
      test_ukm_recorder().GetEntriesByName(InstallUkmEntry::kEntryName);
  ASSERT_EQ(entries.size(), 1u);

  auto* entry = entries[0];
  test_ukm_recorder().ExpectEntrySourceHasUrl(entry, GetInstallableAppURL());
  ukm::TestAutoSetUkmRecorder::ExpectEntryMetric(
      entry, InstallUkmEntry::kIsFullyInstalledName, true);
  ukm::TestAutoSetUkmRecorder::ExpectEntryMetric(
      entry, InstallUkmEntry::kIsPartiallyInstalledName, false);
}

IN_PROC_BROWSER_TEST_F(MLPromotionBrowsertest,
                       PartiallyInstalledAppMeasurement) {
  NavigateAndAwaitInstallabilityCheck(GetInstallableAppURL());
  EXPECT_TRUE(InstallAppForCurrentWebContents(/*install_locally=*/false));

  NavigateAndAwaitInstallabilityCheck(GetUrlWithNoManifest());
  // A re-navigation should retrigger the ML pipeline.
  NavigateAndAwaitMetricsCollectionPending(GetInstallableAppURL());
  task_runner_->RunPendingTasks();

  auto entries =
      test_ukm_recorder().GetEntriesByName(InstallUkmEntry::kEntryName);
  ASSERT_EQ(entries.size(), 1u);

  auto* entry = entries[0];
  test_ukm_recorder().ExpectEntrySourceHasUrl(entry, GetInstallableAppURL());
  ukm::TestAutoSetUkmRecorder::ExpectEntryMetric(
      entry, InstallUkmEntry::kIsFullyInstalledName, false);
  ukm::TestAutoSetUkmRecorder::ExpectEntryMetric(
      entry, InstallUkmEntry::kIsPartiallyInstalledName, true);
}

// SiteQualityMetrics tests.
#if BUILDFLAG(IS_MAC) || BUILDFLAG(IS_LINUX)
// TODO(crbug.com/1450421): Fix the flakiness of the test.
#define MAYBE_SiteQualityMetrics_ServiceWorker_FetchHandler \
  DISABLED_SiteQualityMetrics_ServiceWorker_FetchHandler
#else
#define MAYBE_SiteQualityMetrics_ServiceWorker_FetchHandler \
  SiteQualityMetrics_ServiceWorker_FetchHandler
#endif
IN_PROC_BROWSER_TEST_F(MLPromotionBrowsertest,
                       MAYBE_SiteQualityMetrics_ServiceWorker_FetchHandler) {
  NavigateAndAwaitMetricsCollectionPending(GetInstallableAppURL());
  AwaitServiceWorkerRegistrationAndPendingDelayedTask(GetInstallableAppURL());
  task_runner_->RunPendingTasks();

  auto entries = test_ukm_recorder().GetEntries(
      QualityUkmEntry::kEntryName,
      {QualityUkmEntry::kHasFetchHandlerName,
       QualityUkmEntry::kServiceWorkerScriptSizeName});
  ASSERT_EQ(entries.size(), 1u);

  auto entry = entries[0];
  EXPECT_EQ(test_ukm_recorder().GetSourceForSourceId(entry.source_id)->url(),
            GetInstallableAppURL());
  EXPECT_EQ(entry.metrics[QualityUkmEntry::kHasFetchHandlerName], /*true=*/1);
  EXPECT_GT(entry.metrics[QualityUkmEntry::kServiceWorkerScriptSizeName], 0);
}

IN_PROC_BROWSER_TEST_F(MLPromotionBrowsertest,
                       SiteQualityMetrics_NoServiceWorker_NoFetchHandler) {
  ukm::TestAutoSetUkmRecorder ukm_recorder;
  NavigateAndAwaitMetricsCollectionPending(GetUrlWithNoSWNoFetchHandler());
  task_runner_->RunPendingTasks();

  auto entries = test_ukm_recorder().GetEntries(
      QualityUkmEntry::kEntryName,
      {QualityUkmEntry::kHasFetchHandlerName,
       QualityUkmEntry::kServiceWorkerScriptSizeName});
  ASSERT_EQ(entries.size(), 1u);

  auto entry = entries[0];
  EXPECT_EQ(test_ukm_recorder().GetSourceForSourceId(entry.source_id)->url(),
            GetUrlWithNoSWNoFetchHandler());
  EXPECT_EQ(entry.metrics[QualityUkmEntry::kHasFetchHandlerName], /*false=*/0);
  // Non-existence of a service worker is shown by a script size of 0.
  EXPECT_EQ(entry.metrics[QualityUkmEntry::kServiceWorkerScriptSizeName], 0);
}

#if BUILDFLAG(IS_MAC)
// TODO(crbug.com/1450786): Fix the flakiness of the test.
#define MAYBE_SiteQualityMetrics_ServiceWorker_EmptyFetchHandler \
  DISABLED_SiteQualityMetrics_ServiceWorker_EmptyFetchHandler
#else
#define MAYBE_SiteQualityMetrics_ServiceWorker_EmptyFetchHandler \
  SiteQualityMetrics_ServiceWorker_EmptyFetchHandler
#endif
IN_PROC_BROWSER_TEST_F(
    MLPromotionBrowsertest,
    MAYBE_SiteQualityMetrics_ServiceWorker_EmptyFetchHandler) {
  NavigateAndAwaitMetricsCollectionPending(GetUrlWithSWEmptyFetchHandler());
  AwaitServiceWorkerRegistrationAndPendingDelayedTask(
      GetUrlWithSWEmptyFetchHandler());
  task_runner_->RunPendingTasks();

  // An empty fetch handler is also treated as an existence of a fetch handler.
  auto entries = test_ukm_recorder().GetEntries(
      QualityUkmEntry::kEntryName,
      {QualityUkmEntry::kHasFetchHandlerName,
       QualityUkmEntry::kServiceWorkerScriptSizeName});
  ASSERT_EQ(entries.size(), 1u);

  auto entry = entries[0];
  EXPECT_EQ(test_ukm_recorder().GetSourceForSourceId(entry.source_id)->url(),
            GetUrlWithSWEmptyFetchHandler());
  EXPECT_EQ(entry.metrics[QualityUkmEntry::kHasFetchHandlerName], /*true=*/1);
  EXPECT_GT(entry.metrics[QualityUkmEntry::kServiceWorkerScriptSizeName], 0);
}

#if BUILDFLAG(IS_MAC)
// TODO(crbug.com/1450786): Fix the flakiness of the test.
#define MAYBE_SiteQualityMetrics_ServiceWorker_NoFetchHandler \
  DISABLED_SiteQualityMetrics_ServiceWorker_NoFetchHandler
#else
#define MAYBE_SiteQualityMetrics_ServiceWorker_NoFetchHandler \
  SiteQualityMetrics_ServiceWorker_NoFetchHandler
#endif
IN_PROC_BROWSER_TEST_F(MLPromotionBrowsertest,
                       MAYBE_SiteQualityMetrics_ServiceWorker_NoFetchHandler) {
  NavigateAndAwaitMetricsCollectionPending(GetUrlWithSwNoFetchHandler());
  AwaitServiceWorkerRegistrationAndPendingDelayedTask(
      GetUrlWithSwNoFetchHandler());
  task_runner_->RunPendingTasks();

  auto entries = test_ukm_recorder().GetEntries(
      QualityUkmEntry::kEntryName,
      {QualityUkmEntry::kHasFetchHandlerName,
       QualityUkmEntry::kServiceWorkerScriptSizeName});
  ASSERT_EQ(entries.size(), 1u);

  auto entry = entries[0];
  EXPECT_EQ(test_ukm_recorder().GetSourceForSourceId(entry.source_id)->url(),
            GetUrlWithSwNoFetchHandler());
  EXPECT_EQ(entry.metrics[QualityUkmEntry::kHasFetchHandlerName], /*false=*/0);
  EXPECT_GT(entry.metrics[QualityUkmEntry::kServiceWorkerScriptSizeName], 0);
}

IN_PROC_BROWSER_TEST_F(MLPromotionBrowsertest, PageLoadsWithOnly1Favicon) {
  NavigateAndAwaitMetricsCollectionPending(GetInstallableAppURL());
  task_runner_->RunPendingTasks();

  auto entries = test_ukm_recorder().GetEntries(
      QualityUkmEntry::kEntryName, {QualityUkmEntry::kHasFaviconsName});
  ASSERT_EQ(entries.size(), 1u);

  auto entry = entries[0];
  EXPECT_EQ(test_ukm_recorder().GetSourceForSourceId(entry.source_id)->url(),
            GetInstallableAppURL());
  EXPECT_EQ(entry.metrics[QualityUkmEntry::kHasFaviconsName], 1);
}

// TODO(b/285361272): Add tests for:
// 1. Favicon URL updates.
// 2. Cache storage sizes.

// TODO(b/287255120) : Implement ways of measuring ML outputs on Android.
IN_PROC_BROWSER_TEST_F(MLPromotionBrowsertest, MlInstallNotShown) {
  NavigateAndAwaitMetricsCollectionPending(GetInstallableAppURL());

  ExpectClasificationCallReturnResult(/*site_url=*/GetInstallableAppURL(),
                                      /*manifest_id=*/GetInstallableAppURL(),
                                      "DontShow", TrainingRequestId(1ll));

  // This calls unblocks the metrics tasks, allowing ML to be called.
  task_runner_->RunPendingTasks();

  provider().command_manager().AwaitAllCommandsCompleteForTesting();

  EXPECT_TRUE(provider().registrar_unsafe().is_empty());
}

IN_PROC_BROWSER_TEST_F(MLPromotionBrowsertest, MlInstallShownCancelled) {
  NavigateAndAwaitMetricsCollectionPending(GetInstallableAppURL());

  ExpectClasificationCallReturnResult(
      /*site_url=*/GetInstallableAppURL(),
      /*manifest_id=*/GetInstallableAppURL(),
      MLInstallabilityPromoter::kShowInstallPromptLabel,
      TrainingRequestId(1ll));

  views::NamedWidgetShownWaiter waiter(views::test::AnyWidgetTestPasskey{},
                                       "PWAConfirmationBubbleView");
  // This calls unblocks the metrics tasks, allowing ML to be called.
  task_runner_->RunPendingTasks();
  views::Widget* widget = waiter.WaitIfNeededAndGet();

  ExpectTrainingResult(TrainingRequestId(1ll), MlInstallResponse::kCancelled);

  views::test::CancelDialog(widget);

  provider().command_manager().AwaitAllCommandsCompleteForTesting();

  EXPECT_TRUE(provider().registrar_unsafe().is_empty());
}

IN_PROC_BROWSER_TEST_F(MLPromotionBrowsertest,
                       MlInstallShownIgnoredNavigation) {
  NavigateAndAwaitMetricsCollectionPending(GetInstallableAppURL());

  ExpectClasificationCallReturnResult(
      /*site_url=*/GetInstallableAppURL(),
      /*manifest_id=*/GetInstallableAppURL(),
      MLInstallabilityPromoter::kShowInstallPromptLabel,
      TrainingRequestId(1ll));

  views::NamedWidgetShownWaiter waiter(views::test::AnyWidgetTestPasskey{},
                                       "PWAConfirmationBubbleView");
  // This calls unblocks the metrics tasks, allowing ML to be called.
  task_runner_->RunPendingTasks();
  views::Widget* widget = waiter.WaitIfNeededAndGet();

  ExpectTrainingResult(TrainingRequestId(1ll), MlInstallResponse::kIgnored);

  views::test::WidgetDestroyedWaiter destroyed(widget);
  web_app::NavigateToURLAndWait(browser(), GURL(url::kAboutBlankURL));
  destroyed.Wait();

  provider().command_manager().AwaitAllCommandsCompleteForTesting();

  EXPECT_TRUE(provider().registrar_unsafe().is_empty());
}

IN_PROC_BROWSER_TEST_F(MLPromotionBrowsertest,
                       MlInstallShownIgnoredWidgetClosed) {
  NavigateAndAwaitMetricsCollectionPending(GetInstallableAppURL());

  ExpectClasificationCallReturnResult(
      /*site_url=*/GetInstallableAppURL(),
      /*manifest_id=*/GetInstallableAppURL(),
      MLInstallabilityPromoter::kShowInstallPromptLabel,
      TrainingRequestId(1ll));

  views::NamedWidgetShownWaiter waiter(views::test::AnyWidgetTestPasskey{},
                                       "PWAConfirmationBubbleView");
  // This calls unblocks the metrics tasks, allowing ML to be called.
  task_runner_->RunPendingTasks();
  views::Widget* widget = waiter.WaitIfNeededAndGet();
  views::test::WidgetDestroyedWaiter destroyed(widget);
  ExpectTrainingResult(TrainingRequestId(1ll), MlInstallResponse::kIgnored);
  widget->Close();
  destroyed.Wait();

  provider().command_manager().AwaitAllCommandsCompleteForTesting();

  EXPECT_TRUE(provider().registrar_unsafe().is_empty());
}

IN_PROC_BROWSER_TEST_F(MLPromotionBrowsertest, MlInstallShownAccepted) {
  NavigateAndAwaitMetricsCollectionPending(GetInstallableAppURL());

  ExpectClasificationCallReturnResult(
      /*site_url=*/GetInstallableAppURL(),
      /*manifest_id=*/GetInstallableAppURL(),
      MLInstallabilityPromoter::kShowInstallPromptLabel,
      TrainingRequestId(1ll));

  views::NamedWidgetShownWaiter waiter(views::test::AnyWidgetTestPasskey{},
                                       "PWAConfirmationBubbleView");
  // This calls unblocks the metrics tasks, allowing ML to be called.
  task_runner_->RunPendingTasks();
  views::Widget* widget = waiter.WaitIfNeededAndGet();
  views::test::WidgetDestroyedWaiter destroyed(widget);
  ExpectTrainingResult(TrainingRequestId(1ll), MlInstallResponse::kAccepted);
  views::test::AcceptDialog(widget);
  destroyed.Wait();

  provider().command_manager().AwaitAllCommandsCompleteForTesting();

  EXPECT_FALSE(provider().registrar_unsafe().is_empty());
  web_app::AppId app_id = provider().registrar_unsafe().GetAppIds()[0];
  EXPECT_EQ("Manifest test app",
            provider().registrar_unsafe().GetAppShortName(app_id));
}

IN_PROC_BROWSER_TEST_F(MLPromotionBrowsertest, MlNotShownAlreadyInstalled) {
  NavigateAndAwaitMetricsCollectionPending(GetInstallableAppURL());

  InstallAppForCurrentWebContents(/*install_locally=*/true);

  EXPECT_CALL(*GetMockSegmentation(), GetClassificationResult(_, _, _, _))
      .Times(0);

  // This calls unblocks the metrics tasks, allowing ML to be called. It should
  // not, though, as the app is installed.
  task_runner_->RunPendingTasks();

  provider().command_manager().AwaitAllCommandsCompleteForTesting();
}

IN_PROC_BROWSER_TEST_F(MLPromotionBrowsertest, MlHandlesInvisible) {
  NavigateAndAwaitMetricsCollectionPending(GetInstallableAppURL());

  MLInstallabilityPromoter* original_tab_promoter = ml_promoter();
  content::WebContents* original_web_contents = web_contents();

  // Creating a new tab should ensure that visibility changes.
  chrome::NewTab(browser());

  ExpectClasificationCallReturnResult(
      /*site_url=*/GetInstallableAppURL(),
      /*manifest_id=*/GetInstallableAppURL(),
      MLInstallabilityPromoter::kShowInstallPromptLabel, TrainingRequestId(1ll),
      original_web_contents);

  // This calls unblocks the metrics tasks, allowing ML to be called. However,
  // because the web contents isn't visible, the results won't be reported yet.
  task_runner_->RunPendingTasks();
  EXPECT_TRUE(original_tab_promoter->IsPendingVisibilityForTesting());
  provider().command_manager().AwaitAllCommandsCompleteForTesting();
  EXPECT_TRUE(provider().registrar_unsafe().is_empty());

  // Navigating to the previous tab will resume the installation UX reporting,
  // so handle installation request.
  views::NamedWidgetShownWaiter waiter(views::test::AnyWidgetTestPasskey{},
                                       "PWAConfirmationBubbleView");
  ExpectTrainingResult(TrainingRequestId(1ll), MlInstallResponse::kAccepted,
                       original_web_contents);
  chrome::SelectPreviousTab(browser());
  views::Widget* widget = waiter.WaitIfNeededAndGet();
  views::test::WidgetDestroyedWaiter destroyed(widget);
  views::test::AcceptDialog(widget);
  destroyed.Wait();
  provider().command_manager().AwaitAllCommandsCompleteForTesting();

  EXPECT_FALSE(provider().registrar_unsafe().is_empty());
}

IN_PROC_BROWSER_TEST_F(MLPromotionBrowsertest, MlInstallGuardrailBlocked) {
  NavigateAndAwaitMetricsCollectionPending(GetInstallableAppURL());

  ExpectClasificationCallReturnResult(
      /*site_url=*/GetInstallableAppURL(),
      /*manifest_id=*/GetInstallableAppURL(),
      MLInstallabilityPromoter::kShowInstallPromptLabel,
      TrainingRequestId(1ll));

  views::NamedWidgetShownWaiter waiter(views::test::AnyWidgetTestPasskey{},
                                       "PWAConfirmationBubbleView");
  // This calls unblocks the metrics tasks, allowing ML to be called.
  task_runner_->RunPendingTasks();

  // Cancelling the dialog will save that result in the guardrails, which should
  // cause the next immediate install call to trigger the guardrail response.
  views::Widget* widget = waiter.WaitIfNeededAndGet();
  ExpectTrainingResult(TrainingRequestId(1ll), MlInstallResponse::kCancelled);
  views::test::WidgetDestroyedWaiter destroyed(widget);
  views::test::CancelDialog(widget);
  destroyed.Wait();

  // Ensure that nothing is installed.
  provider().command_manager().AwaitAllCommandsCompleteForTesting();
  EXPECT_TRUE(provider().registrar_unsafe().is_empty());

  web_app::NavigateToURLAndWait(browser(), GURL(url::kAboutBlankURL));

  // Test that guardrails now block the install.
  NavigateAndAwaitMetricsCollectionPending(GetInstallableAppURL());

  ExpectClasificationCallReturnResult(
      /*site_url=*/GetInstallableAppURL(),
      /*manifest_id=*/GetInstallableAppURL(),
      MLInstallabilityPromoter::kShowInstallPromptLabel,
      TrainingRequestId(2ll));
  // This will cause the ML pipeline to complete, but not report anything yet.
  // This allows the user install to possibly still happen and report success.
  task_runner_->RunPendingTasks();

  ExpectTrainingResult(TrainingRequestId(2ll),
                       MlInstallResponse::kBlockedGuardrails, web_contents());
  // Doing another navigation should now trigger the guardrail blocked signal.
  web_app::NavigateToURLAndWait(browser(), GURL(url::kAboutBlankURL));
}

IN_PROC_BROWSER_TEST_F(MLPromotionBrowsertest,
                       MlInstallGuardrailIgnoredUserInstallAccepted) {
  NavigateAndAwaitMetricsCollectionPending(GetInstallableAppURL());

  ExpectClasificationCallReturnResult(
      /*site_url=*/GetInstallableAppURL(),
      /*manifest_id=*/GetInstallableAppURL(),
      MLInstallabilityPromoter::kShowInstallPromptLabel,
      TrainingRequestId(1ll));

  // Cancelling the dialog will save that result in the guardrails, which
  // should cause the next immediate install call to trigger the guardrail
  // response.
  {
    views::NamedWidgetShownWaiter waiter(views::test::AnyWidgetTestPasskey{},
                                         "PWAConfirmationBubbleView");
    // This calls unblocks the metrics tasks, allowing ML to be called.
    task_runner_->RunPendingTasks();

    views::Widget* widget = waiter.WaitIfNeededAndGet();
    ExpectTrainingResult(TrainingRequestId(1ll), MlInstallResponse::kCancelled);
    views::test::WidgetDestroyedWaiter destroyed(widget);
    views::test::CancelDialog(widget);
    destroyed.Wait();
  }
  // Ensure that nothing is installed.
  provider().command_manager().AwaitAllCommandsCompleteForTesting();
  EXPECT_TRUE(provider().registrar_unsafe().is_empty());

  web_app::NavigateToURLAndWait(browser(), GURL(url::kAboutBlankURL));

  // Navigate back to the app url to re-trigger the ml pipeline.
  ExpectClasificationCallReturnResult(
      /*site_url=*/GetInstallableAppURL(),
      /*manifest_id=*/GetInstallableAppURL(),
      MLInstallabilityPromoter::kShowInstallPromptLabel,
      TrainingRequestId(2ll));
  NavigateAndAwaitMetricsCollectionPending(GetInstallableAppURL());
  task_runner_->RunPendingTasks();

  // Test that the guardrail isn't reported when the user completes the install,
  // and instead reports success.
  ExpectTrainingResult(TrainingRequestId(2ll), MlInstallResponse::kAccepted);
  EXPECT_TRUE(InstallAppFromUserInitiation(/*accept_install=*/true));
}

IN_PROC_BROWSER_TEST_F(MLPromotionBrowsertest,
                       MlInstallGuardrailIgnoredUserInstallCancelled) {
  NavigateAndAwaitMetricsCollectionPending(GetInstallableAppURL());

  ExpectClasificationCallReturnResult(
      /*site_url=*/GetInstallableAppURL(),
      /*manifest_id=*/GetInstallableAppURL(),
      MLInstallabilityPromoter::kShowInstallPromptLabel,
      TrainingRequestId(1ll));

  // Cancelling the dialog will save that result in the guardrails, which
  // should cause the next immediate install call to trigger the guardrail
  // response.
  {
    views::NamedWidgetShownWaiter waiter(views::test::AnyWidgetTestPasskey{},
                                         "PWAConfirmationBubbleView");
    // This calls unblocks the metrics tasks, allowing ML to be called.
    task_runner_->RunPendingTasks();

    views::Widget* widget = waiter.WaitIfNeededAndGet();
    ExpectTrainingResult(TrainingRequestId(1ll), MlInstallResponse::kCancelled);
    views::test::WidgetDestroyedWaiter destroyed(widget);
    views::test::CancelDialog(widget);
    destroyed.Wait();
  }
  // Ensure that nothing is installed.
  provider().command_manager().AwaitAllCommandsCompleteForTesting();
  EXPECT_TRUE(provider().registrar_unsafe().is_empty());

  web_app::NavigateToURLAndWait(browser(), GURL(url::kAboutBlankURL));

  // Navigate back to the app url to re-trigger the ml pipeline.
  ExpectClasificationCallReturnResult(
      /*site_url=*/GetInstallableAppURL(),
      /*manifest_id=*/GetInstallableAppURL(),
      MLInstallabilityPromoter::kShowInstallPromptLabel,
      TrainingRequestId(2ll));
  NavigateAndAwaitMetricsCollectionPending(GetInstallableAppURL());
  task_runner_->RunPendingTasks();

  // Test that the guardrail isn't reported when the user completes the install,
  // and instead reports success.
  ExpectTrainingResult(TrainingRequestId(2ll), MlInstallResponse::kCancelled);
  EXPECT_TRUE(InstallAppFromUserInitiation(/*accept_install=*/false));
}

}  // namespace
}  // namespace webapps
