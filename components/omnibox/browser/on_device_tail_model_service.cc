// Copyright 2023 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "components/omnibox/browser/on_device_tail_model_service.h"

#include <utility>

#include "base/containers/flat_set.h"
#include "base/files/file_path.h"
#include "base/functional/bind.h"
#include "base/functional/callback.h"
#include "base/logging.h"
#include "base/task/task_traits.h"
#include "base/task/thread_pool.h"
#include "components/omnibox/browser/on_device_tail_model_executor.h"
#include "components/optimization_guide/core/optimization_guide_model_provider.h"
#include "components/optimization_guide/core/optimization_guide_util.h"
#include "components/optimization_guide/proto/models.pb.h"
#include "components/optimization_guide/proto/on_device_tail_suggest_model_metadata.pb.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

namespace {

void InitializeTailModelExecutor(
    OnDeviceTailModelExecutor* executor,
    const base::FilePath& model_file,
    const base::flat_set<base::FilePath>& additional_files,
    const optimization_guide::proto::OnDeviceTailSuggestModelMetadata&
        metadata) {
  if (executor == nullptr) {
    return;
  }
  if (model_file.empty() || additional_files.empty()) {
    return;
  }

  base::FilePath vocab_filepath;
  for (const base::FilePath& file_path : additional_files) {
    if (!file_path.empty()) {
      // Currently only one additional file (i.e. vocabulary) will be sent.
      vocab_filepath = file_path;
      break;
    }
  }

  if (vocab_filepath.empty()) {
    return;
  }

  executor->Init(model_file, vocab_filepath, metadata);
}

std::vector<OnDeviceTailModelExecutor::Prediction> RunTailModelExecutor(
    OnDeviceTailModelExecutor* executor,
    const OnDeviceTailModelService::OnDeviceTailModelInput& input) {
  std::vector<OnDeviceTailModelExecutor::Prediction> predictions;
  if (executor == nullptr || !executor->IsReady()) {
    return predictions;
  }
  predictions = executor->GenerateSuggestionsForPrefix(
      input.sanitized_input, input.previous_query, input.max_num_suggestions,
      input.max_num_step, input.probability_threshold);
  return predictions;
}

}  // namespace

OnDeviceTailModelService::OnDeviceTailModelService(
    optimization_guide::OptimizationGuideModelProvider* model_provider)
    : model_executor_task_runner_(base::ThreadPool::CreateSequencedTaskRunner(
          {base::MayBlock(), base::TaskPriority::BEST_EFFORT})),
      tail_model_executor_(
          new OnDeviceTailModelExecutor(),
          base::OnTaskRunnerDeleter(model_executor_task_runner_)),
      model_provider_(model_provider) {
  if (model_provider_) {
    model_provider_->AddObserverForOptimizationTargetModel(
        optimization_guide::proto::
            OPTIMIZATION_TARGET_OMNIBOX_ON_DEVICE_TAIL_SUGGEST,
        /* model_metadata= */ absl::nullopt, this);
  }
}

OnDeviceTailModelService::~OnDeviceTailModelService() {
  if (model_provider_) {
    model_provider_->RemoveObserverForOptimizationTargetModel(
        optimization_guide::proto::
            OPTIMIZATION_TARGET_OMNIBOX_ON_DEVICE_TAIL_SUGGEST,
        this);
    model_provider_ = nullptr;
  }
}

void OnDeviceTailModelService::OnModelUpdated(
    optimization_guide::proto::OptimizationTarget optimization_target,
    const optimization_guide::ModelInfo& model_info) {
  if (optimization_target !=
      optimization_guide::proto::
          OPTIMIZATION_TARGET_OMNIBOX_ON_DEVICE_TAIL_SUGGEST) {
    return;
  }

  const absl::optional<optimization_guide::proto::Any>& metadata =
      model_info.GetModelMetadata();
  absl::optional<optimization_guide::proto::OnDeviceTailSuggestModelMetadata>
      tail_model_metadata = absl::nullopt;
  if (metadata.has_value()) {
    tail_model_metadata = optimization_guide::ParsedAnyMetadata<
        optimization_guide::proto::OnDeviceTailSuggestModelMetadata>(
        metadata.value());
  }

  if (!tail_model_metadata.has_value()) {
    DVLOG(1) << "Failed to fetch metadata for Omnibox on device tail model";
    return;
  }

  model_executor_task_runner_->PostTask(
      FROM_HERE,
      base::BindOnce(&InitializeTailModelExecutor, tail_model_executor_.get(),
                     model_info.GetModelFilePath(),
                     model_info.GetAdditionalFiles(),
                     tail_model_metadata.value()));
}

void OnDeviceTailModelService::GetPredictionsForInput(
    const OnDeviceTailModelInput& input,
    ResultCallback result_callback) {
  model_executor_task_runner_->PostTaskAndReplyWithResult(
      FROM_HERE,
      base::BindOnce(&RunTailModelExecutor, tail_model_executor_.get(), input),
      std::move(result_callback));
}
