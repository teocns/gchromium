// Copyright 2023 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_EXTENSIONS_SMART_CARD_PROVIDER_PRIVATE_SMART_CARD_PROVIDER_PRIVATE_API_H_
#define CHROME_BROWSER_CHROMEOS_EXTENSIONS_SMART_CARD_PROVIDER_PRIVATE_SMART_CARD_PROVIDER_PRIVATE_API_H_

#include "base/memory/raw_ref.h"
#include "base/memory/weak_ptr.h"
#include "base/sequence_checker.h"
#include "base/time/time.h"
#include "base/types/id_type.h"
#include "extensions/browser/browser_context_keyed_api_factory.h"
#include "extensions/browser/extension_function.h"
#include "mojo/public/cpp/bindings/pending_remote.h"
#include "mojo/public/cpp/bindings/receiver.h"
#include "mojo/public/cpp/bindings/receiver_set.h"
#include "services/device/public/mojom/smart_card.mojom.h"

namespace extensions {
struct Event;
class EventRouter;

// Implements device::mojom::SmartCardContextFactory (and the other
// device::mojom::SmartCard interfaces) by talking to the extension that is
// listening to chrome.smartCardProviderPrivate events.
class SmartCardProviderPrivateAPI
    : public BrowserContextKeyedAPI,
      public device::mojom::SmartCardContextFactory,
      public device::mojom::SmartCardContext,
      public device::mojom::SmartCardConnection {
 public:
  // Uniquely identifies a request sent by this class to the PC/SC provider
  // extension.
  using RequestId = base::IdType32<class SmartCardRequestIdClass>;
  // A smart card context id, as given by the PC/SC provider extension.
  using ContextId = base::IdType32<class SmartCardContextIdClass>;
  // A smart card handle. Identifies a connection in the PC/SC provider
  // extension.
  using Handle = base::IdType32<class SmartCardHandleClass>;

  using ResultArgs = std::variant<
      std::monostate,            // ReleaseContext, Cancel
      ContextId,                 // EstablishContext
      std::vector<std::string>,  // ListReaders
      std::vector<
          device::mojom::SmartCardReaderStateOutPtr>,         // GetStatusChange
      std::tuple<Handle, device::mojom::SmartCardProtocol>>;  // Connect

  using SmartCardCallback = std::variant<
      // Cancel, Disconnect
      base::OnceCallback<void(device::mojom::SmartCardResultPtr)>,
      ListReadersCallback,
      GetStatusChangeCallback,
      ConnectCallback,
      CreateContextCallback>;

  using ProcessResultCallback = base::OnceCallback<
      void(ResultArgs, device::mojom::SmartCardResultPtr, SmartCardCallback)>;

  static BrowserContextKeyedAPIFactory<SmartCardProviderPrivateAPI>*
  GetFactoryInstance();

  // Convenience method to get the SmartCardProviderPrivateAPI for a
  // BrowserContext.
  static SmartCardProviderPrivateAPI& Get(content::BrowserContext& context);

  explicit SmartCardProviderPrivateAPI(content::BrowserContext* context);

  ~SmartCardProviderPrivateAPI() override;

  mojo::PendingRemote<device::mojom::SmartCardContextFactory>
  GetSmartCardContextFactory();

  // device::mojom::SmartCardContextFactory overrides:
  void CreateContext(CreateContextCallback) override;

  // device::mojom::SmartCardContext overrides:
  void ListReaders(ListReadersCallback callback) override;
  void GetStatusChange(
      base::TimeDelta timeout,
      std::vector<device::mojom::SmartCardReaderStateInPtr> reader_states,
      GetStatusChangeCallback callback) override;
  void Cancel(CancelCallback callback) override;
  void Connect(const std::string& reader,
               device::mojom::SmartCardShareMode share_mode,
               device::mojom::SmartCardProtocolsPtr preferred_protocols,
               ConnectCallback callback) override;
  void Disconnect(device::mojom::SmartCardDisposition disposition,
                  DisconnectCallback callback) override;

  // Called by extension functions:
  void ReportResult(RequestId request_id,
                    ResultArgs result_args,
                    device::mojom::SmartCardResultPtr result);
  void ReportReleaseContextResult(RequestId request_id,
                                  device::mojom::SmartCardResultPtr result);
  void ReportEstablishContextResult(RequestId request_id,
                                    ContextId scard_context,
                                    device::mojom::SmartCardResultPtr result);
  void ReportConnectResult(RequestId request_id,
                           Handle,
                           device::mojom::SmartCardProtocol,
                           device::mojom::SmartCardResultPtr result);

  void SetResponseTimeLimitForTesting(base::TimeDelta);

  using DisconnectObserver = base::RepeatingClosure;
  void SetDisconnectObserverForTesting(DisconnectObserver observer);

 private:
  // BrowserContextKeyedAPI:
  static const bool kServiceIsCreatedWithBrowserContext = false;
  static const char* service_name() { return "SmartCardProviderPrivateAPI"; }

  friend class BrowserContextKeyedAPIFactory<SmartCardProviderPrivateAPI>;

  // Called by ReportResult:
  void ProcessListReadersResult(ResultArgs result_args,
                                device::mojom::SmartCardResultPtr result,
                                SmartCardCallback callback);
  void ProcessGetStatusChangeResult(ResultArgs result_args,
                                    device::mojom::SmartCardResultPtr result,
                                    SmartCardCallback callback);
  void ProcessCancelResult(ResultArgs result_args,
                           device::mojom::SmartCardResultPtr result,
                           SmartCardCallback callback);
  void ProcessConnectResult(ContextId scard_context,
                            ResultArgs result_args,
                            device::mojom::SmartCardResultPtr result,
                            SmartCardCallback callback);
  void ProcessDisconnectResult(ResultArgs result_args,
                               device::mojom::SmartCardResultPtr result,
                               SmartCardCallback callback);

  // If the context is free the request is run immediately.
  // Otherwise it is put in a task queue.
  void RunOrQueueRequest(ContextId scard_context, base::OnceClosure request);

  // Methods to send requests to the smart card provider extension:
  void SendReleaseContext(ContextId scard_context);
  void SendListReaders(ContextId scard_context, ListReadersCallback callback);
  void SendGetStatusChange(
      ContextId scard_context,
      base::TimeDelta time_delta,
      std::vector<device::mojom::SmartCardReaderStateInPtr> reader_states,
      GetStatusChangeCallback callback);
  void SendConnect(ContextId scard_context,
                   const std::string& reader,
                   device::mojom::SmartCardShareMode share_mode,
                   device::mojom::SmartCardProtocolsPtr preferred_protocols,
                   ConnectCallback callback);
  void SendDisconnect(ContextId scard_context,
                      Handle handle,
                      device::mojom::SmartCardDisposition disposition,
                      DisconnectCallback callback);

  // Called when a device::mojom::SmartCardContext loses its mojo connection.
  // eg: because its mojo Remote was destroyed.
  void OnMojoContextDisconnected();

  // Called when a device::mojom::SmartCardConnection loses its mojo connection.
  // eg: because its mojo Remote was destroyed.
  void OnMojoConnectionDisconnected();

  void OnScardHandleDisconnected(device::mojom::SmartCardResultPtr result);

  void RunNextRequestForContext(ContextId scard_context);

  std::string GetListenerExtensionId(const extensions::Event& event);

  void OnEstablishContextTimeout(const std::string& provider_extension_id,
                                 RequestId request_id);
  void OnReleaseContextTimeout(const std::string& provider_extension_id,
                               RequestId request_id);
  void OnListReadersTimeout(const std::string& provider_extension_id,
                            RequestId request_id);
  void OnGetStatusChangeTimeout(const std::string& provider_extension_id,
                                RequestId request_id);
  void OnCancelTimeout(const std::string& provider_extension_id,
                       RequestId request_id);
  void OnConnectTimeout(const std::string& provider_extension_id,
                        RequestId request_id);
  void OnDisconnectTimeout(const std::string& provider_extension_id,
                           RequestId request_id);

  template <typename ResultPtr>
  void DispatchEventWithTimeout(
      ContextId scard_context,
      const std::string& event_name,
      extensions::events::HistogramValue histogram_value,
      ProcessResultCallback process_result,
      base::OnceCallback<void(ResultPtr)> callback,
      void (SmartCardProviderPrivateAPI::*OnTimeout)(const std::string&,
                                                     RequestId),
      base::Value::List event_arguments = base::Value::List(),
      absl::optional<base::TimeDelta> timeout = absl::nullopt);

  device::mojom::SmartCardConnectResultPtr CreateSmartCardConnection(
      ContextId scard_context,
      Handle handle,
      device::mojom::SmartCardProtocol active_protocol);

  struct ContextData;

  ContextData& GetContextData(ContextId scard_context);

  // Returns true is we are waiting for the provider to send back the result
  // of a request sent on that context. Ie, we have a pending result on that
  // context.
  bool IsContextBusy(ContextId scard_context) const;

  SEQUENCE_CHECKER(sequence_checker_);

  base::TimeDelta response_time_limit_{base::Minutes(5)};

  struct PendingResult;
  std::map<RequestId, std::unique_ptr<PendingResult>> pending_results_;

  RequestId::Generator request_id_generator_;
  const raw_ref<content::BrowserContext> browser_context_;
  const raw_ref<EventRouter> event_router_;

  std::map<ContextId, ContextData> context_data_map_;

  mojo::ReceiverSet<device::mojom::SmartCardContextFactory>
      context_factory_receivers_;

  mojo::ReceiverSet<device::mojom::SmartCardContext, ContextId>
      context_receivers_;

  mojo::ReceiverSet<device::mojom::SmartCardConnection,
                    std::tuple<ContextId, Handle>>
      connection_receivers_;

  DisconnectObserver disconnect_observer_;

  base::WeakPtrFactory<SmartCardProviderPrivateAPI> weak_ptr_factory_{this};
};

class SmartCardProviderPrivateReportEstablishContextResultFunction
    : public ExtensionFunction {
 private:
  // ExtensionFunction:
  ~SmartCardProviderPrivateReportEstablishContextResultFunction() override;
  ResponseAction Run() override;

  DECLARE_EXTENSION_FUNCTION(
      "smartCardProviderPrivate.reportEstablishContextResult",
      SMARTCARDPROVIDERPRIVATE_REPORTESTABLISHCONTEXTRESULT)
};

class SmartCardProviderPrivateReportReleaseContextResultFunction
    : public ExtensionFunction {
 private:
  // ExtensionFunction:
  ~SmartCardProviderPrivateReportReleaseContextResultFunction() override;
  ResponseAction Run() override;

  DECLARE_EXTENSION_FUNCTION(
      "smartCardProviderPrivate.reportReleaseContextResult",
      SMARTCARDPROVIDERPRIVATE_REPORTRELEASECONTEXTRESULT)
};

class SmartCardProviderPrivateReportListReadersResultFunction
    : public ExtensionFunction {
 private:
  // ExtensionFunction:
  ~SmartCardProviderPrivateReportListReadersResultFunction() override;
  ResponseAction Run() override;

  DECLARE_EXTENSION_FUNCTION("smartCardProviderPrivate.reportListReadersResult",
                             SMARTCARDPROVIDERPRIVATE_REPORTLISTREADERSRESULT)
};

class SmartCardProviderPrivateReportGetStatusChangeResultFunction
    : public ExtensionFunction {
 private:
  // ExtensionFunction:
  ~SmartCardProviderPrivateReportGetStatusChangeResultFunction() override;
  ResponseAction Run() override;

  DECLARE_EXTENSION_FUNCTION(
      "smartCardProviderPrivate.reportGetStatusChangeResult",
      SMARTCARDPROVIDERPRIVATE_REPORTGETSTATUSCHANGERESULT)
};

class SmartCardProviderPrivateReportCancelResultFunction
    : public ExtensionFunction {
 private:
  // ExtensionFunction:
  ~SmartCardProviderPrivateReportCancelResultFunction() override;
  ResponseAction Run() override;

  DECLARE_EXTENSION_FUNCTION("smartCardProviderPrivate.reportCancelResult",
                             SMARTCARDPROVIDERPRIVATE_REPORTCANCELRESULT)
};

class SmartCardProviderPrivateReportConnectResultFunction
    : public ExtensionFunction {
 private:
  // ExtensionFunction:
  ~SmartCardProviderPrivateReportConnectResultFunction() override;
  ResponseAction Run() override;

  DECLARE_EXTENSION_FUNCTION("smartCardProviderPrivate.reportConnectResult",
                             SMARTCARDPROVIDERPRIVATE_REPORTCONNECTRESULT)
};

class SmartCardProviderPrivateReportDisconnectResultFunction
    : public ExtensionFunction {
 private:
  // ExtensionFunction:
  ~SmartCardProviderPrivateReportDisconnectResultFunction() override;
  ResponseAction Run() override;

  DECLARE_EXTENSION_FUNCTION("smartCardProviderPrivate.reportDisconnectResult",
                             SMARTCARDPROVIDERPRIVATE_REPORTDISCONNECTRESULT)
};

}  // namespace extensions

#endif  // CHROME_BROWSER_CHROMEOS_EXTENSIONS_SMART_CARD_PROVIDER_PRIVATE_SMART_CARD_PROVIDER_PRIVATE_API_H_
