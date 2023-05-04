// Copyright 2023 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/browser/chromeos/extensions/smart_card_provider_private/smart_card_provider_private_api.h"

#include <queue>
#include <variant>

#include "base/no_destructor.h"
#include "base/notreached.h"
#include "base/timer/timer.h"
#include "chrome/common/extensions/api/smart_card_provider_private.h"
#include "content/public/browser/render_frame_host.h"
#include "content/public/browser/web_contents.h"
#include "extensions/browser/event_listener_map.h"
#include "extensions/browser/event_router.h"
#include "extensions/browser/event_router_factory.h"
#include "extensions/browser/extension_event_histogram_value.h"
#include "mojo/public/cpp/bindings/pending_remote.h"
#include "services/device/public/mojom/smart_card.mojom.h"

namespace scard_api = extensions::api::smart_card_provider_private;

using device::mojom::SmartCardConnectResult;
using device::mojom::SmartCardCreateContextResult;
using device::mojom::SmartCardCreateContextResultPtr;
using device::mojom::SmartCardError;
using device::mojom::SmartCardListReadersResult;
using device::mojom::SmartCardResult;
using device::mojom::SmartCardResultPtr;
using device::mojom::SmartCardStatusChangeResult;
using device::mojom::SmartCardSuccess;

namespace {
device::mojom::SmartCardResultPtr ProviderResultCodeToSmartCardResult(
    scard_api::ResultCode code) {
  switch (code) {
    case scard_api::RESULT_CODE_NONE:
      return SmartCardResult::NewError(SmartCardError::kUnknown);
    case scard_api::RESULT_CODE_SUCCESS:
      return SmartCardResult::NewSuccess(SmartCardSuccess::kOk);
    case scard_api::RESULT_CODE_REMOVED_CARD:
      return SmartCardResult::NewError(SmartCardError::kRemovedCard);
    case scard_api::RESULT_CODE_RESET_CARD:
      return SmartCardResult::NewError(SmartCardError::kResetCard);
    case scard_api::RESULT_CODE_UNPOWERED_CARD:
      return SmartCardResult::NewError(SmartCardError::kUnpoweredCard);
    case scard_api::RESULT_CODE_UNRESPONSIVE_CARD:
      return SmartCardResult::NewError(SmartCardError::kUnresponsiveCard);
    case scard_api::RESULT_CODE_UNSUPPORTED_CARD:
      return SmartCardResult::NewError(SmartCardError::kUnsupportedCard);
    case scard_api::RESULT_CODE_READER_UNAVAILABLE:
      return SmartCardResult::NewError(SmartCardError::kReaderUnavailable);
    case scard_api::RESULT_CODE_SHARING_VIOLATION:
      return SmartCardResult::NewError(SmartCardError::kSharingViolation);
    case scard_api::RESULT_CODE_NOT_TRANSACTED:
      return SmartCardResult::NewError(SmartCardError::kNotTransacted);
    case scard_api::RESULT_CODE_NO_SMARTCARD:
      return SmartCardResult::NewError(SmartCardError::kNoSmartcard);
    case scard_api::RESULT_CODE_PROTO_MISMATCH:
      return SmartCardResult::NewError(SmartCardError::kProtoMismatch);
    case scard_api::RESULT_CODE_SYSTEM_CANCELLED:
      return SmartCardResult::NewError(SmartCardError::kSystemCancelled);
    case scard_api::RESULT_CODE_NOT_READY:
      return SmartCardResult::NewError(SmartCardError::kNotReady);
    case scard_api::RESULT_CODE_CANCELLED:
      return SmartCardResult::NewError(SmartCardError::kCancelled);
    case scard_api::RESULT_CODE_INSUFFICIENT_BUFFER:
      return SmartCardResult::NewError(SmartCardError::kInsufficientBuffer);
    case scard_api::RESULT_CODE_INVALID_HANDLE:
      return SmartCardResult::NewError(SmartCardError::kInvalidHandle);
    case scard_api::RESULT_CODE_INVALID_PARAMETER:
      return SmartCardResult::NewError(SmartCardError::kInvalidParameter);
    case scard_api::RESULT_CODE_INVALID_VALUE:
      return SmartCardResult::NewError(SmartCardError::kInvalidValue);
    case scard_api::RESULT_CODE_NO_MEMORY:
      return SmartCardResult::NewError(SmartCardError::kNoMemory);
    case scard_api::RESULT_CODE_TIMEOUT:
      return SmartCardResult::NewError(SmartCardError::kTimeout);
    case scard_api::RESULT_CODE_UNKNOWN_READER:
      return SmartCardResult::NewError(SmartCardError::kUnknownReader);
    case scard_api::RESULT_CODE_UNSUPPORTED_FEATURE:
      return SmartCardResult::NewError(SmartCardError::kUnsupportedFeature);
    case scard_api::RESULT_CODE_NO_READERS_AVAILABLE:
      return SmartCardResult::NewError(SmartCardError::kNoReadersAvailable);
    case scard_api::RESULT_CODE_SERVICE_STOPPED:
      return SmartCardResult::NewError(SmartCardError::kServiceStopped);
    case scard_api::RESULT_CODE_NO_SERVICE:
      return SmartCardResult::NewError(SmartCardError::kNoService);
    case scard_api::RESULT_CODE_COMM_ERROR:
      return SmartCardResult::NewError(SmartCardError::kCommError);
    case scard_api::RESULT_CODE_INTERNAL_ERROR:
      return SmartCardResult::NewError(SmartCardError::kInternalError);
    case scard_api::RESULT_CODE_UNKNOWN_ERROR:
      return SmartCardResult::NewError(SmartCardError::kUnknownError);
    case scard_api::RESULT_CODE_SERVER_TOO_BUSY:
      return SmartCardResult::NewError(SmartCardError::kServerTooBusy);
    case scard_api::RESULT_CODE_UNEXPECTED:
      return SmartCardResult::NewError(SmartCardError::kUnexpected);
    case scard_api::RESULT_CODE_SHUTDOWN:
      return SmartCardResult::NewError(SmartCardError::kShutdown);
    case scard_api::RESULT_CODE_UNKNOWN:
      return SmartCardResult::NewError(SmartCardError::kUnknown);
  }
}

device::mojom::SmartCardReaderStateFlagsPtr ToSmartCardProviderReaderStateFlags(
    const scard_api::ReaderStateFlags& flags) {
  auto result = device::mojom::SmartCardReaderStateFlags::New();

#define CONVERT_FLAG(flag)             \
  if (flags.flag.has_value()) {        \
    result->flag = flags.flag.value(); \
  }

  CONVERT_FLAG(unaware)
  CONVERT_FLAG(ignore)
  CONVERT_FLAG(changed)
  CONVERT_FLAG(unknown)
  CONVERT_FLAG(unavailable)
  CONVERT_FLAG(empty)
  CONVERT_FLAG(present)
  CONVERT_FLAG(exclusive)
  CONVERT_FLAG(inuse)
  CONVERT_FLAG(mute)
  CONVERT_FLAG(unpowered)

#undef CONVERT_FLAG
  return result;
}

std::vector<device::mojom::SmartCardReaderStateOutPtr>
ToSmartCardProviderReaderStateOutVector(
    std::vector<scard_api::ReaderStateOut>& reader_states) {
  std::vector<device::mojom::SmartCardReaderStateOutPtr> result_vector;
  result_vector.reserve(reader_states.size());

  for (auto& reader_state : reader_states) {
    auto mojom_reader_state = device::mojom::SmartCardReaderStateOut::New();

    mojom_reader_state->reader = std::move(reader_state.reader);
    mojom_reader_state->event_state =
        ToSmartCardProviderReaderStateFlags(reader_state.event_state);
    mojom_reader_state->answer_to_reset = std::move(reader_state.atr);

    result_vector.push_back(std::move(mojom_reader_state));
  }

  return result_vector;
}

base::Value::Dict ToValue(
    const device::mojom::SmartCardReaderStateFlags& state_flags) {
  base::Value::Dict to_value_result;

  scard_api::ReaderStateFlags result;

#define CONVERT_FLAG(flag) result.flag = state_flags.flag;

  CONVERT_FLAG(unaware)
  CONVERT_FLAG(ignore)
  CONVERT_FLAG(changed)
  CONVERT_FLAG(unknown)
  CONVERT_FLAG(unavailable)
  CONVERT_FLAG(empty)
  CONVERT_FLAG(present)
  CONVERT_FLAG(exclusive)
  CONVERT_FLAG(inuse)
  CONVERT_FLAG(mute)
  CONVERT_FLAG(unpowered)

#undef CONVERT_FLAG

  return result.ToValue();
}

base::Value::Dict ToValue(
    const device::mojom::SmartCardReaderStateIn& state_in) {
  base::Value::Dict to_value_result;

  to_value_result.Set("reader", state_in.reader);
  to_value_result.Set("currentState", ToValue(*state_in.current_state.get()));

  return to_value_result;
}

scard_api::ShareMode ToApiShareMode(
    device::mojom::SmartCardShareMode share_mode) {
  switch (share_mode) {
    case device::mojom::SmartCardShareMode::kShared:
      return scard_api::SHARE_MODE_SHARED;
    case device::mojom::SmartCardShareMode::kExclusive:
      return scard_api::SHARE_MODE_EXCLUSIVE;
    case device::mojom::SmartCardShareMode::kDirect:
      return scard_api::SHARE_MODE_DIRECT;
  }
}

base::Value ToValue(device::mojom::SmartCardShareMode share_mode) {
  return base::Value(scard_api::ToString(ToApiShareMode(share_mode)));
}

base::Value::Dict ToValue(const device::mojom::SmartCardProtocols& protocols) {
  scard_api::Protocols result;

  result.t0 = protocols.t0;
  result.t1 = protocols.t1;
  result.raw = protocols.raw;

  return result.ToValue();
}

scard_api::Disposition ToApiDisposition(
    device::mojom::SmartCardDisposition disposition) {
  switch (disposition) {
    case device::mojom::SmartCardDisposition::kLeave:
      return scard_api::DISPOSITION_LEAVE_CARD;
    case device::mojom::SmartCardDisposition::kReset:
      return scard_api::DISPOSITION_RESET_CARD;
    case device::mojom::SmartCardDisposition::kUnpower:
      return scard_api::DISPOSITION_UNPOWER_CARD;
    case device::mojom::SmartCardDisposition::kEject:
      return scard_api::DISPOSITION_EJECT_CARD;
  }
}

base::Value ToValue(device::mojom::SmartCardDisposition disposition) {
  return base::Value(scard_api::ToString(ToApiDisposition(disposition)));
}

device::mojom::SmartCardProtocol ToDeviceMojomSmartCardProtocol(
    scard_api::Protocol protocol) {
  switch (protocol) {
    case scard_api::PROTOCOL_NONE:
    case scard_api::PROTOCOL_UNDEFINED:
      return device::mojom::SmartCardProtocol::kUndefined;
    case scard_api::PROTOCOL_T0:
      return device::mojom::SmartCardProtocol::kT0;
    case scard_api::PROTOCOL_T1:
      return device::mojom::SmartCardProtocol::kT1;
    case scard_api::PROTOCOL_RAW:
      return device::mojom::SmartCardProtocol::kRaw;
  }
}

template <class PendingType>
std::unique_ptr<PendingType> Extract(
    std::map<extensions::SmartCardProviderPrivateAPI::RequestId,
             std::unique_ptr<PendingType>>& pending_map,
    extensions::SmartCardProviderPrivateAPI::RequestId request_id) {
  auto it = pending_map.find(request_id);
  if (it == pending_map.end()) {
    return nullptr;
  }

  std::unique_ptr<PendingType> pending = std::move(it->second);
  CHECK(pending);

  pending_map.erase(it);

  return pending;
}

}  // namespace

namespace extensions {

struct SmartCardProviderPrivateAPI::PendingResult {
  PendingResult() = default;
  ~PendingResult() = default;

  base::OneShotTimer timer;
  ContextId scard_context;  // Can be invalid (null).

  ProcessResultCallback process_result;
  SmartCardCallback callback;
};

// Information about an established scard_context
struct SmartCardProviderPrivateAPI::ContextData {
  ContextData() = default;
  ContextData(ContextData&&) = default;
  ContextData& operator=(ContextData&&) = default;
  ContextData(const ContextData&) = delete;
  ContextData& operator=(const ContextData&) = delete;

  // A PC/SC context can only handle one request at a time (exception being
  // SCardCancel()).
  // Thus we have to make sure not to send a request on a ContextId which has an
  // ongoing call (ie, a request was sent to the provider but its result was not
  // yet received).
  //
  // This queue contains requests from device::mojom::SmartCardContext or
  // device::mojom::SmartCardConnection for this context that have arrived
  // while it was waiting for the result of a previous request.
  std::queue<base::OnceClosure> task_queue;

  // All device::mojom::SmartCardConnection receivers created on this context.
  std::set<mojo::ReceiverId> connection_receiver_ids;
};

// static
BrowserContextKeyedAPIFactory<SmartCardProviderPrivateAPI>*
SmartCardProviderPrivateAPI::GetFactoryInstance() {
  static base::NoDestructor<
      BrowserContextKeyedAPIFactory<SmartCardProviderPrivateAPI>>
      instance;
  return instance.get();
}

// static
SmartCardProviderPrivateAPI& SmartCardProviderPrivateAPI::Get(
    content::BrowserContext& context) {
  return *GetFactoryInstance()->Get(&context);
}

SmartCardProviderPrivateAPI::SmartCardProviderPrivateAPI(
    content::BrowserContext* context)
    : browser_context_(raw_ref<content::BrowserContext>::from_ptr(context)),
      event_router_(raw_ref<EventRouter>::from_ptr(
          EventRouterFactory::GetForBrowserContext(context))) {
  context_receivers_.set_disconnect_handler(base::BindRepeating(
      &SmartCardProviderPrivateAPI::OnMojoContextDisconnected,
      weak_ptr_factory_.GetWeakPtr()));

  connection_receivers_.set_disconnect_handler(base::BindRepeating(
      &SmartCardProviderPrivateAPI::OnMojoConnectionDisconnected,
      weak_ptr_factory_.GetWeakPtr()));
}

SmartCardProviderPrivateAPI::~SmartCardProviderPrivateAPI() = default;

mojo::PendingRemote<device::mojom::SmartCardContextFactory>
SmartCardProviderPrivateAPI::GetSmartCardContextFactory() {
  mojo::PendingRemote<device::mojom::SmartCardContextFactory> pending_remote;
  context_factory_receivers_.Add(
      this, pending_remote.InitWithNewPipeAndPassReceiver());
  return pending_remote;
}

void SmartCardProviderPrivateAPI::CreateContext(
    CreateContextCallback callback) {
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);

  DispatchEventWithTimeout(
      ContextId(),  // This call is requesting a new context, not passing an
                    // existing one.
      scard_api::OnEstablishContextRequested::kEventName,
      extensions::events::
          SMART_CARD_PROVIDER_PRIVATE_ON_ESTABLISH_CONTEXT_REQUESTED,
      ProcessResultCallback(),  // It has its own ReportEstablishContextResult
                                // method.
      std::move(callback),
      &SmartCardProviderPrivateAPI::OnEstablishContextTimeout);
}

void SmartCardProviderPrivateAPI::OnMojoContextDisconnected() {
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);
  if (disconnect_observer_) {
    base::SequencedTaskRunner::GetCurrentDefault()->PostTask(
        FROM_HERE, disconnect_observer_);
  }

  const ContextId scard_context = context_receivers_.current_context();
  CHECK(scard_context);

  ContextData& context_data = GetContextData(scard_context);

  // Disconnect all mojom::SmartCardConnection receivers created on this context
  // as their handles will all become invalid at PC/SC level once the context
  // is released.
  for (mojo::ReceiverId connection_receiver_id :
       context_data.connection_receiver_ids) {
    connection_receivers_.Remove(connection_receiver_id);
  }

  RunOrQueueRequest(
      scard_context,
      base::BindOnce(&SmartCardProviderPrivateAPI::SendReleaseContext,
                     weak_ptr_factory_.GetWeakPtr(), scard_context));
}

void SmartCardProviderPrivateAPI::OnMojoConnectionDisconnected() {
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);

  auto callback =
      base::BindOnce(&SmartCardProviderPrivateAPI::OnScardHandleDisconnected,
                     weak_ptr_factory_.GetWeakPtr());

  const auto& [context_id, handle] = connection_receivers_.current_context();
  CHECK(context_id);
  CHECK(handle);

  RunOrQueueRequest(
      context_id,
      base::BindOnce(&SmartCardProviderPrivateAPI::SendDisconnect,
                     weak_ptr_factory_.GetWeakPtr(), context_id, handle,
                     device::mojom::SmartCardDisposition::kLeave,
                     std::move(callback)));
}

void SmartCardProviderPrivateAPI::OnScardHandleDisconnected(
    device::mojom::SmartCardResultPtr result) {
  if (result->is_error()) {
    LOG(WARNING) << "Failed to disconnect SCard Handle: "
                 << result->get_error();
  }
}

void SmartCardProviderPrivateAPI::RunOrQueueRequest(ContextId scard_context,
                                                    base::OnceClosure request) {
  if (IsContextBusy(scard_context)) {
    GetContextData(scard_context).task_queue.push(std::move(request));
    return;
  }

  std::move(request).Run();
}

void SmartCardProviderPrivateAPI::SendReleaseContext(ContextId scard_context) {
  RequestId request_id = request_id_generator_.GenerateNextId();

  base::Value::List event_args;
  event_args.Append(request_id.GetUnsafeValue());
  event_args.Append(scard_context.GetUnsafeValue());

  auto event = std::make_unique<extensions::Event>(
      extensions::events::
          SMART_CARD_PROVIDER_PRIVATE_ON_RELEASE_CONTEXT_REQUESTED,
      scard_api::OnReleaseContextRequested::kEventName, std::move(event_args),
      &*browser_context_);

  const std::string provider_extension_id = GetListenerExtensionId(*event);

  if (provider_extension_id.empty()) {
    return;
  }

  auto pending = std::make_unique<PendingResult>();
  pending->scard_context = scard_context;
  pending->timer.Start(
      FROM_HERE, response_time_limit_,
      base::BindOnce(&SmartCardProviderPrivateAPI::OnReleaseContextTimeout,
                     weak_ptr_factory_.GetWeakPtr(), provider_extension_id,
                     request_id));

  pending_results_[request_id] = std::move(pending);

  event_router_->DispatchEventToExtension(provider_extension_id,
                                          std::move(event));
}

void SmartCardProviderPrivateAPI::SendDisconnect(
    ContextId scard_context,
    Handle handle,
    device::mojom::SmartCardDisposition disposition,
    DisconnectCallback callback) {
  auto process_result =
      base::BindOnce(&SmartCardProviderPrivateAPI::ProcessDisconnectResult,
                     weak_ptr_factory_.GetWeakPtr());

  DispatchEventWithTimeout(
      scard_context, scard_api::OnDisconnectRequested::kEventName,
      extensions::events::SMART_CARD_PROVIDER_PRIVATE_ON_DISCONNECT_REQUESTED,
      std::move(process_result), std::move(callback),
      &SmartCardProviderPrivateAPI::OnDisconnectTimeout,
      /*event_arguments=*/
      base::Value::List()
          .Append(handle.GetUnsafeValue())
          .Append(ToValue(disposition)));
}

void SmartCardProviderPrivateAPI::ReportResult(
    RequestId request_id,
    ResultArgs result_args,
    device::mojom::SmartCardResultPtr result) {
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);

  std::unique_ptr<PendingResult> pending =
      Extract(pending_results_, request_id);
  if (!pending) {
    return;
  }

  std::move(pending->process_result)
      .Run(std::move(result_args), std::move(result),
           std::move(pending->callback));

  // 'Cancel' does not affect the task queue for its context.
  // Thus it won't set an scard_context on its `PendingResult`.
  if (pending->scard_context) {
    RunNextRequestForContext(pending->scard_context);
  }
}

void SmartCardProviderPrivateAPI::ReportEstablishContextResult(
    RequestId request_id,
    ContextId scard_context,
    SmartCardResultPtr result) {
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);

  std::unique_ptr<PendingResult> pending =
      Extract(pending_results_, request_id);
  if (!pending) {
    if (result->is_success() && scard_context) {
      LOG(WARNING) << "Releasing scard_context from an unknown "
                      "EstablishContext request.";
      SendReleaseContext(scard_context);
    }
    return;
  }

  SmartCardCreateContextResultPtr context_result;

  if (result->is_success()) {
    if (scard_context) {
      mojo::PendingRemote<device::mojom::SmartCardContext> context_remote;
      context_receivers_.Add(
          this, context_remote.InitWithNewPipeAndPassReceiver(), scard_context);
      context_result =
          SmartCardCreateContextResult::NewContext(std::move(context_remote));
    } else {
      LOG(ERROR) << "Provider reported an invalid scard_context value: "
                 << scard_context.GetUnsafeValue();
      // Just ignore this result.
      context_result = SmartCardCreateContextResult::NewError(
          SmartCardError::kInternalError);
    }
  } else {
    context_result =
        SmartCardCreateContextResult::NewError(result->get_error());
  }

  CHECK(std::holds_alternative<CreateContextCallback>(pending->callback));
  std::move(std::get<CreateContextCallback>(pending->callback))
      .Run(std::move(context_result));

  // It's neither expected nor supported for the provider to recycle a
  // scard_context value so soon.
  CHECK(!context_data_map_.contains(scard_context));

  context_data_map_[scard_context] = ContextData();
}

void SmartCardProviderPrivateAPI::ReportReleaseContextResult(
    RequestId request_id,
    SmartCardResultPtr result) {
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);

  std::unique_ptr<PendingResult> pending =
      Extract(pending_results_, request_id);
  if (!pending) {
    return;
  }

  if (result->is_error()) {
    // There's nothing really to be done about it.
    LOG(WARNING) << "Failed to release context: " << result->get_error();
  }

  auto it = context_data_map_.find(pending->scard_context);
  CHECK(it != context_data_map_.end());
  context_data_map_.erase(it);
}

void SmartCardProviderPrivateAPI::ProcessListReadersResult(
    ResultArgs result_args,
    SmartCardResultPtr result,
    SmartCardCallback callback) {
  CHECK(std::holds_alternative<std::vector<std::string>>(result_args));
  auto readers = std::move(std::get<std::vector<std::string>>(result_args));

  CHECK(std::holds_alternative<ListReadersCallback>(callback));
  std::move(std::get<ListReadersCallback>(callback))
      .Run(result->is_success()
               ? SmartCardListReadersResult::NewReaders(std::move(readers))
               : SmartCardListReadersResult::NewError(result->get_error()));
}

void SmartCardProviderPrivateAPI::ProcessGetStatusChangeResult(
    ResultArgs result_args,
    SmartCardResultPtr result,
    SmartCardCallback callback) {
  device::mojom::SmartCardStatusChangeResultPtr status_change_result;

  CHECK(std::holds_alternative<
        std::vector<device::mojom::SmartCardReaderStateOutPtr>>(result_args));
  auto reader_states = std::move(
      std::get<std::vector<device::mojom::SmartCardReaderStateOutPtr>>(
          result_args));

  if (result->is_success()) {
    status_change_result =
        SmartCardStatusChangeResult::NewReaderStates(std::move(reader_states));
  } else {
    status_change_result =
        SmartCardStatusChangeResult::NewError(result->get_error());
  }

  CHECK(std::holds_alternative<GetStatusChangeCallback>(callback));
  std::move(std::get<GetStatusChangeCallback>(callback))
      .Run(std::move(status_change_result));
}

void SmartCardProviderPrivateAPI::ProcessCancelResult(
    ResultArgs result_args,
    SmartCardResultPtr result,
    SmartCardCallback callback) {
  CHECK(std::holds_alternative<CancelCallback>(callback));
  std::move(std::get<CancelCallback>(callback)).Run(std::move(result));
}

device::mojom::SmartCardConnectResultPtr
SmartCardProviderPrivateAPI::CreateSmartCardConnection(
    ContextId scard_context,
    Handle handle,
    device::mojom::SmartCardProtocol active_protocol) {
  if (handle.is_null()) {
    LOG(ERROR) << "Provider reported an invalid handle value: "
               << handle.GetUnsafeValue();
    // Just ignore this result.
    return SmartCardConnectResult::NewError(SmartCardError::kInternalError);
  }

  mojo::PendingRemote<device::mojom::SmartCardConnection> connection_remote;
  mojo::ReceiverId connection_receiver_id = connection_receivers_.Add(
      this, connection_remote.InitWithNewPipeAndPassReceiver(),
      std::make_tuple(scard_context, handle));

  GetContextData(scard_context)
      .connection_receiver_ids.insert(connection_receiver_id);

  return SmartCardConnectResult::NewSuccess(
      device::mojom::SmartCardConnectSuccess::New(std::move(connection_remote),
                                                  active_protocol));
}

void SmartCardProviderPrivateAPI::ReportConnectResult(
    RequestId request_id,
    Handle handle,
    device::mojom::SmartCardProtocol active_protocol,
    device::mojom::SmartCardResultPtr result) {
  if (!pending_results_.contains(request_id)) {
    // TODO(crbug.com/1386175): send disconnect request to PC/SC provider if
    // the handle is valid and the result is success to avoid leaking this
    // seemingly unrequested connection.
    return;
  }

  ReportResult(request_id, std::make_tuple(handle, active_protocol),
               std::move(result));
}

void SmartCardProviderPrivateAPI::ProcessConnectResult(
    ContextId scard_context,
    ResultArgs result_args,
    device::mojom::SmartCardResultPtr result,
    SmartCardCallback callback) {
  device::mojom::SmartCardConnectResultPtr connect_result;
  auto handle_and_protocol =
      std::get<std::tuple<Handle, device::mojom::SmartCardProtocol>>(
          result_args);

  if (result->is_success()) {
    connect_result = CreateSmartCardConnection(
        scard_context, std::get<Handle>(handle_and_protocol),
        std::get<device::mojom::SmartCardProtocol>(handle_and_protocol));
  } else {
    connect_result = SmartCardConnectResult::NewError(result->get_error());
  }

  CHECK(std::holds_alternative<ConnectCallback>(callback));
  std::move(std::get<ConnectCallback>(callback)).Run(std::move(connect_result));
}

void SmartCardProviderPrivateAPI::RunNextRequestForContext(
    ContextId scard_context) {
  auto it = context_data_map_.find(scard_context);
  CHECK(it != context_data_map_.end());

  ContextData& context_data = it->second;

  // Context must be free, since this method is called after a pending request
  // finishes processing the received result.
  CHECK(!IsContextBusy(scard_context));

  if (context_data.task_queue.empty()) {
    return;
  }

  auto task = std::move(context_data.task_queue.front());
  context_data.task_queue.pop();
  std::move(task).Run();
}

void SmartCardProviderPrivateAPI::ProcessDisconnectResult(
    ResultArgs result_args,
    device::mojom::SmartCardResultPtr result,
    SmartCardCallback callback) {
  CHECK(std::holds_alternative<DisconnectCallback>(callback));
  std::move(std::get<DisconnectCallback>(callback)).Run(std::move(result));
}

void SmartCardProviderPrivateAPI::SetResponseTimeLimitForTesting(
    base::TimeDelta value) {
  response_time_limit_ = value;
}

void SmartCardProviderPrivateAPI::SetDisconnectObserverForTesting(
    DisconnectObserver observer) {
  disconnect_observer_ = observer;
}

// TODO(crbug.com/1386175): Consider if we need to wait for a known
// SmartCard provider Extension to load or finish installation
// before querying for listeners.
// Use case is if the Web API is used immediately after a user logs
// in.
std::string SmartCardProviderPrivateAPI::GetListenerExtensionId(
    const extensions::Event& event) {
  std::set<const extensions::EventListener*> listener_set =
      event_router_->listeners().GetEventListeners(event);

  if (listener_set.empty()) {
    LOG(ERROR) << "No extension listening to " << event.event_name << ".";
    return std::string();
  }

  // Allow list on the extension API permission enforces that there can't
  // be multiple extensions with access to it. Thus don't bother
  // iterating through the set.
  return (*listener_set.cbegin())->extension_id();
}

template <typename ResultPtr>
void SmartCardProviderPrivateAPI::DispatchEventWithTimeout(
    ContextId scard_context,
    const std::string& event_name,
    extensions::events::HistogramValue histogram_value,
    ProcessResultCallback process_result,
    base::OnceCallback<void(ResultPtr)> callback,
    void (SmartCardProviderPrivateAPI::*OnTimeout)(const std::string&,
                                                   RequestId),
    base::Value::List event_arguments,
    absl::optional<base::TimeDelta> timeout) {
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);

  RequestId request_id = request_id_generator_.GenerateNextId();

  event_arguments.Insert(event_arguments.begin(),
                         base::Value(request_id.GetUnsafeValue()));

  auto event = std::make_unique<extensions::Event>(histogram_value, event_name,
                                                   std::move(event_arguments),
                                                   &*browser_context_);

  const std::string provider_extension_id = GetListenerExtensionId(*event);
  if (provider_extension_id.empty()) {
    ResultPtr error(absl::in_place);
    error->set_error(SmartCardError::kNoService);
    std::move(callback).Run(std::move(error));
    return;
  }

  auto pending = std::make_unique<PendingResult>();
  pending->scard_context = scard_context;
  pending->callback = std::move(callback);
  pending->process_result = std::move(process_result);
  pending->timer.Start(FROM_HERE,
                       timeout ? timeout.value() : response_time_limit_,
                       base::BindOnce(OnTimeout, weak_ptr_factory_.GetWeakPtr(),
                                      provider_extension_id, request_id));

  pending_results_[request_id] = std::move(pending);

  event_router_->DispatchEventToExtension(provider_extension_id,
                                          std::move(event));
}

void SmartCardProviderPrivateAPI::ListReaders(ListReadersCallback callback) {
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);

  const ContextId scard_context = context_receivers_.current_context();
  CHECK(scard_context);

  RunOrQueueRequest(
      scard_context,
      base::BindOnce(&SmartCardProviderPrivateAPI::SendListReaders,
                     weak_ptr_factory_.GetWeakPtr(), scard_context,
                     std::move(callback)));
}

void SmartCardProviderPrivateAPI::SendListReaders(
    ContextId scard_context,
    ListReadersCallback callback) {
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);
  CHECK(scard_context);

  base::Value::List event_args;
  event_args.Append(scard_context.GetUnsafeValue());

  auto process_result =
      base::BindOnce(&SmartCardProviderPrivateAPI::ProcessListReadersResult,
                     weak_ptr_factory_.GetWeakPtr());

  DispatchEventWithTimeout(
      scard_context, scard_api::OnListReadersRequested::kEventName,
      extensions::events::SMART_CARD_PROVIDER_PRIVATE_ON_LIST_READERS_REQUESTED,
      std::move(process_result), std::move(callback),
      &SmartCardProviderPrivateAPI::OnListReadersTimeout,
      std::move(event_args));
}

void SmartCardProviderPrivateAPI::GetStatusChange(
    base::TimeDelta time_delta,
    std::vector<device::mojom::SmartCardReaderStateInPtr> reader_states,
    GetStatusChangeCallback callback) {
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);

  const ContextId scard_context = context_receivers_.current_context();
  DCHECK(!scard_context.is_null());

  RunOrQueueRequest(
      scard_context,
      base::BindOnce(&SmartCardProviderPrivateAPI::SendGetStatusChange,
                     weak_ptr_factory_.GetWeakPtr(), scard_context, time_delta,
                     std::move(reader_states), std::move(callback)));
}

void SmartCardProviderPrivateAPI::SendGetStatusChange(
    ContextId scard_context,
    base::TimeDelta time_delta,
    std::vector<device::mojom::SmartCardReaderStateInPtr> reader_states,
    GetStatusChangeCallback callback) {
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);
  CHECK(scard_context);

  base::Value::List event_args;
  event_args.Append(scard_context.GetUnsafeValue());

  const bool finite_timeout =
      !time_delta.is_inf() &&
      time_delta.InMilliseconds() < int64_t(std::numeric_limits<int>::max());

  scard_api::Timeout timeout;
  if (finite_timeout) {
    timeout.milliseconds = int(time_delta.InMilliseconds());
  }
  event_args.Append(timeout.ToValue());

  base::Value::List reader_states_list;
  for (const auto& reader_state : reader_states) {
    reader_states_list.Append(ToValue(*reader_state.get()));
  }
  event_args.Append(std::move(reader_states_list));

  auto process_result =
      base::BindOnce(&SmartCardProviderPrivateAPI::ProcessGetStatusChangeResult,
                     weak_ptr_factory_.GetWeakPtr());

  DispatchEventWithTimeout(
      scard_context, scard_api::OnGetStatusChangeRequested::kEventName,
      extensions::events::
          SMART_CARD_PROVIDER_PRIVATE_ON_GET_STATUS_CHANGE_REQUESTED,
      std::move(process_result), std::move(callback),
      &SmartCardProviderPrivateAPI::OnGetStatusChangeTimeout,
      std::move(event_args), std::max(base::Milliseconds(500), time_delta * 2));
}

void SmartCardProviderPrivateAPI::Cancel(CancelCallback callback) {
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);

  const ContextId scard_context = context_receivers_.current_context();
  CHECK(!scard_context.is_null());

  auto process_result =
      base::BindOnce(&SmartCardProviderPrivateAPI::ProcessCancelResult,
                     weak_ptr_factory_.GetWeakPtr());

  DispatchEventWithTimeout(
      ContextId(),  // Passing an invalid context as Cancel can be called in
                    // parallel to other PC/SC calls and therefore never gets
                    // queued or is affected by the context being in busy state.
      scard_api::OnCancelRequested::kEventName,
      extensions::events::SMART_CARD_PROVIDER_PRIVATE_ON_CANCEL_REQUESTED,
      std::move(process_result), std::move(callback),
      &SmartCardProviderPrivateAPI::OnCancelTimeout,
      /*event_arguments=*/
      base::Value::List().Append(scard_context.GetUnsafeValue()));
}

void SmartCardProviderPrivateAPI::Connect(
    const std::string& reader,
    device::mojom::SmartCardShareMode share_mode,
    device::mojom::SmartCardProtocolsPtr preferred_protocols,
    ConnectCallback callback) {
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);

  const ContextId scard_context = context_receivers_.current_context();
  CHECK(!scard_context.is_null());

  RunOrQueueRequest(
      scard_context,
      base::BindOnce(&SmartCardProviderPrivateAPI::SendConnect,
                     weak_ptr_factory_.GetWeakPtr(), scard_context, reader,
                     share_mode, std::move(preferred_protocols),
                     std::move(callback)));
}

void SmartCardProviderPrivateAPI::SendConnect(
    ContextId scard_context,
    const std::string& reader,
    device::mojom::SmartCardShareMode share_mode,
    device::mojom::SmartCardProtocolsPtr preferred_protocols,
    ConnectCallback callback) {
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);
  CHECK(scard_context);

  base::Value::List event_args;
  event_args.Append(scard_context.GetUnsafeValue());
  event_args.Append(reader);
  event_args.Append(ToValue(share_mode));
  event_args.Append(ToValue(*preferred_protocols.get()));

  auto process_result =
      base::BindOnce(&SmartCardProviderPrivateAPI::ProcessConnectResult,
                     weak_ptr_factory_.GetWeakPtr(), scard_context);

  DispatchEventWithTimeout(
      scard_context, scard_api::OnConnectRequested::kEventName,
      extensions::events::SMART_CARD_PROVIDER_PRIVATE_ON_CONNECT_REQUESTED,
      std::move(process_result), std::move(callback),
      &SmartCardProviderPrivateAPI::OnConnectTimeout, std::move(event_args));
}

void SmartCardProviderPrivateAPI::Disconnect(
    device::mojom::SmartCardDisposition disposition,
    DisconnectCallback callback) {
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);

  const auto& [context_id, handle] = connection_receivers_.current_context();
  CHECK(context_id);
  CHECK(handle);

  RunOrQueueRequest(context_id,
                    base::BindOnce(&SmartCardProviderPrivateAPI::SendDisconnect,
                                   weak_ptr_factory_.GetWeakPtr(), context_id,
                                   handle, disposition, std::move(callback)));
}

SmartCardProviderPrivateAPI::ContextData&
SmartCardProviderPrivateAPI::GetContextData(ContextId scard_context) {
  auto it = context_data_map_.find(scard_context);
  CHECK(it != context_data_map_.end());
  return it->second;
}

bool SmartCardProviderPrivateAPI::IsContextBusy(ContextId scard_context) const {
  // I expect no more than a dozen contexts in a profile at any given time (in
  // most cases much less than that). Thus the cost traversing the map is
  // negligible.
  for (const auto& [request_id, pending_result] : pending_results_) {
    if (pending_result->scard_context == scard_context) {
      return true;
    }
  }
  return false;
}

#define ON_TIMEOUT_IMPL(FunctionName, ReportResultName, ...)              \
  void SmartCardProviderPrivateAPI::On##FunctionName##Timeout(            \
      const std::string& provider_extension_id, RequestId request_id) {   \
    LOG(ERROR) << "Provider extension " << provider_extension_id          \
               << " did not report the result of " << #FunctionName       \
               << " (request id " << request_id.GetUnsafeValue() << ")."; \
    ReportResultName(request_id, __VA_ARGS__);                            \
  }

ON_TIMEOUT_IMPL(ReleaseContext,
                ReportReleaseContextResult,
                SmartCardResult::NewError(SmartCardError::kNoService))

ON_TIMEOUT_IMPL(EstablishContext,
                ReportEstablishContextResult,
                ContextId(),
                SmartCardResult::NewError(SmartCardError::kNoService))

ON_TIMEOUT_IMPL(ListReaders,
                ReportResult,
                std::vector<std::string>(),
                SmartCardResult::NewError(SmartCardError::kNoService))

ON_TIMEOUT_IMPL(GetStatusChange,
                ReportResult,
                std::vector<device::mojom::SmartCardReaderStateOutPtr>(),
                SmartCardResult::NewError(SmartCardError::kNoService))

ON_TIMEOUT_IMPL(Cancel,
                ReportResult,
                std::monostate(),
                SmartCardResult::NewError(SmartCardError::kNoService))

ON_TIMEOUT_IMPL(Connect,
                ReportResult,
                std::make_tuple(Handle(),
                                device::mojom::SmartCardProtocol::kUndefined),
                SmartCardResult::NewError(SmartCardError::kNoService))

ON_TIMEOUT_IMPL(Disconnect,
                ReportResult,
                std::monostate(),
                SmartCardResult::NewError(SmartCardError::kNoService))

#undef ON_TIMEOUT_IMPL

#define REPORT_RESULT_FUNCTION_IMPL(FunctionName, ReportResultName, ...)    \
  SmartCardProviderPrivateReport##FunctionName##ResultFunction::            \
      ~SmartCardProviderPrivateReport##FunctionName##ResultFunction() =     \
          default;                                                          \
  ExtensionFunction::ResponseAction                                         \
      SmartCardProviderPrivateReport##FunctionName##ResultFunction::Run() { \
    absl::optional<scard_api::Report##FunctionName##Result::Params> params( \
        scard_api::Report##FunctionName##Result::Params::Create(args()));   \
    EXTENSION_FUNCTION_VALIDATE(params);                                    \
    if (!params) {                                                          \
      return RespondNow(NoArguments());                                     \
    }                                                                       \
                                                                            \
    SmartCardProviderPrivateAPI::RequestId request_id(params->request_id);  \
                                                                            \
    auto& scard_provider =                                                  \
        SmartCardProviderPrivateAPI::Get(*browser_context());               \
    scard_provider.ReportResultName(request_id, __VA_ARGS__);               \
    return RespondNow(NoArguments());                                       \
  }

REPORT_RESULT_FUNCTION_IMPL(
    ReleaseContext,
    ReportReleaseContextResult,
    ProviderResultCodeToSmartCardResult(params->result_code))

REPORT_RESULT_FUNCTION_IMPL(
    EstablishContext,
    ReportEstablishContextResult,
    SmartCardProviderPrivateAPI::ContextId(params->scard_context),
    ProviderResultCodeToSmartCardResult(params->result_code))

REPORT_RESULT_FUNCTION_IMPL(
    Connect,
    ReportConnectResult,
    SmartCardProviderPrivateAPI::Handle(params->scard_handle),
    ToDeviceMojomSmartCardProtocol(params->active_protocol),
    ProviderResultCodeToSmartCardResult(params->result_code))

REPORT_RESULT_FUNCTION_IMPL(
    ListReaders,
    ReportResult,
    std::move(params->readers),
    ProviderResultCodeToSmartCardResult(params->result_code))

REPORT_RESULT_FUNCTION_IMPL(
    GetStatusChange,
    ReportResult,
    ToSmartCardProviderReaderStateOutVector(params->reader_states),
    ProviderResultCodeToSmartCardResult(params->result_code))

REPORT_RESULT_FUNCTION_IMPL(
    Cancel,
    ReportResult,
    std::monostate(),
    ProviderResultCodeToSmartCardResult(params->result_code))

REPORT_RESULT_FUNCTION_IMPL(
    Disconnect,
    ReportResult,
    std::monostate(),
    ProviderResultCodeToSmartCardResult(params->result_code))

#undef REPORT_RESULT_FUNCTION_IMPL

}  // namespace extensions
