// Copyright 2023 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_METRICS_STRUCTURED_REPORTING_STRUCTURED_METRICS_REPORTING_SERVICE_H_
#define COMPONENTS_METRICS_STRUCTURED_REPORTING_STRUCTURED_METRICS_REPORTING_SERVICE_H_

#include "components/metrics/reporting_service.h"
#include "components/metrics/unsent_log_store.h"

class PrefRegistrySimple;
class PrefService;

namespace metrics {
class MetricsServiceClient;
}

namespace metrics::structured::reporting {

// The limiting parameters of the log store and reporting service.
struct StorageLimits {
  size_t min_log_queue_count = 0;
  size_t min_log_queue_size = 0;
  size_t max_log_size = 0;
};

// A service that uploads Structured Metrics logs to the UMA server.
class StructuredMetricsReportingService : public metrics::ReportingService {
 public:
  StructuredMetricsReportingService(MetricsServiceClient* client,
                                    PrefService* local_state,
                                    const StorageLimits& storage_limits);

  static void RegisterPrefs(PrefRegistrySimple* registry);

 private:
  // metrics::ReportingService:
  metrics::LogStore* log_store() override;

  // Getters for MetricsLogUploader parameters.
  GURL GetUploadUrl() const override;
  GURL GetInsecureUploadUrl() const override;
  base::StringPiece upload_mime_type() const override;
  MetricsLogUploader::MetricServiceType service_type() const override;

  metrics::UnsentLogStore log_store_;
};
}  // namespace metrics::structured::reporting

#endif  // COMPONENTS_METRICS_STRUCTURED_REPORTING_STRUCTURED_METRICS_REPORTING_SERVICE_H_
