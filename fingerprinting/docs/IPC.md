We have set-up a FingerprintManager service whose purpose is to parse a JSON provided by the `--fingerprint` switch, and provide it to various Chromium's context.
Since many of the Chromium's ends run on different processes, we have implemented our manager using IPC:





```
// fingerprint_manager.h
#ifndef FINGERPRINTING_MANAGER_H_
#define FINGERPRINTING_MANAGER_H_

#include <string>

#include "base/files/file_util.h"
#include "base/json/json_reader.h"
#include "base/logging.h"
#include "base/no_destructor.h"
#include "base/strings/string_util.h"
#include "base/synchronization/lock.h"
#include "base/values.h"
#include "fingerprinting/export.h"
#include "fingerprinting/manager/manager.mojom.h"
#include "fingerprinting/core/device_descriptor/fingerprint.h"
#include "third_party/abseil-cpp/absl/types/optional.h"


namespace fingerprinting {

class FINGERPRINTING_EXPORT FingerprintManager : public fingerprinting::mojom::FingerprintingManager {
 public:
  static FingerprintManager* GetInstance();

  void Init(const std::string& fingerprint_file_path);

  void SetFingerprintStr(const std::string& fingerprint, SetFingerprintStrCallback callback) override;

  void GetFingerprintStr(GetFingerprintStrCallback callback) override;

  void Enabled(EnabledCallback callback) override;


  static absl::optional<Fingerprint> FromStringToObj(const std::string& fingerprint_str);

  bool CanBeInitialized(){
    return !initialized_ && can_be_initialized_;
  }

  void SetCanBeInitialized(bool can_be){
    can_be_initialized_ = can_be;
  }
 private:
  friend class base::NoDestructor<FingerprintManager>;

  FingerprintManager();
  ~FingerprintManager() override;

  void LoadFingerprint(const std::string& fingerprint_file_path);

  base::Lock lock_;
  std::unique_ptr<Fingerprint> fingerprint_;

  std::string fingerprint_str_;

  bool initialized_ = false;

  bool loaded_ = false;

  // Usually set to false when switch isn't present
  bool can_be_initialized_ = true;



};

}  // namespace base

#endif  // FINGERPRINTING_MANAGER_H_
```

fingerprint_manager.cc
```
#include <string>
#include "fingerprinting/manager/manager.h"
#include "base/files/file_util.h"
#include "base/json/json_reader.h"
#include "base/logging.h"
#include "base/no_destructor.h"
#include "base/strings/string_util.h"
#include "base/synchronization/lock.h"
#include "base/values.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

namespace fingerprinting {



FingerprintManager::FingerprintManager() = default;
FingerprintManager::~FingerprintManager() = default;

// static
FingerprintManager* FingerprintManager::GetInstance() {
  static base::NoDestructor<FingerprintManager> instance;
  return instance.get();
}

  void FingerprintManager::Init(const std::string& fingerprint_file_path) {
    if (!CanBeInitialized()) {
      return;
    }
    initialized_ = true;
    base::AutoLock auto_lock(lock_);
    LoadFingerprint(fingerprint_file_path);
  }

void FingerprintManager::Enabled(EnabledCallback callback) {
  std::move(callback).Run(loaded_);
}

void FingerprintManager::GetFingerprintStr(GetFingerprintStrCallback callback) {
  base::AutoLock auto_lock(lock_);
  std::move(callback).Run(fingerprint_str_);
}


void FingerprintManager::SetFingerprintStr(const std::string& fingerprint, SetFingerprintStrCallback callback) {
  base::AutoLock auto_lock(lock_);

  absl::optional<Fingerprint> fp = FingerprintManager::FromStringToObj(fingerprint);

  if (!fp.has_value()) {
    std::move(callback).Run(false);
  }
  fingerprint_str_ = std::move(fingerprint);
  fingerprint_ = std::make_unique<Fingerprint>(std::move(fp.value()));

  std::move(callback).Run(true);
}


void FingerprintManager::LoadFingerprint(const std::string& fingerprint_file_path) {
  std::string fingerprint_str;

  // Convert std::string to base::FilePath
  base::FilePath fingerprint_file_path_o = base::FilePath(fingerprint_file_path);
  if (!base::ReadFileToString(fingerprint_file_path_o, &fingerprint_str)) {
    LOG(ERROR) << "Failed to read fingerprint file: " << fingerprint_file_path;
    return;
  }

  absl::optional<Fingerprint> fp = FingerprintManager::FromStringToObj(fingerprint_str);

  if (fp.has_value()) {
    fingerprint_ = std::make_unique<Fingerprint>(std::move(fp.value()));
    fingerprint_str_ = std::move(fingerprint_str);
    loaded_ = true;
    // Print the fingerprint_file_path location
    LOG(INFO) << "Loaded fingerprint from " << fingerprint_file_path;
  }
}

absl::optional<Fingerprint> FingerprintManager::FromStringToObj(const std::string& fingerprint_str) {
  absl::optional<base::Value> fingerprint_value = 
      base::JSONReader::Read(fingerprint_str);

  if (!fingerprint_value.has_value()) {
    LOG(ERROR) << "Failed to parse fingerprint JSON: " << fingerprint_str;
    return absl::nullopt;
  }
  
  base::Value& fingerprint_dict = fingerprint_value.value();

  if (!fingerprint_dict.is_dict()) {
    LOG(ERROR) << "Fingerprint JSON is not a dictionary: " << fingerprint_str;
    return absl::nullopt;
  }

  return Fingerprint(std::move(fingerprint_dict));
}

}  // namespace fingerprinting
```

Then, we needed to create a Service so that it could be consumed by other processes. We have implemented it by extending the FingerprintingManager interface (which already implements mojo), and implementing the methods we need to expose. 


```
#ifndef FINGERPRINTING_MANAGER_SERVICE_CC_
#define FINGERPRINTING_MANAGER_SERVICE_CC_


#include "base/files/file_util.h"
#include "base/json/json_reader.h"
#include "base/logging.h"
#include "base/no_destructor.h"
#include "base/strings/string_util.h"
#include "base/synchronization/lock.h"
#include "base/values.h"
#include "fingerprinting/export.h"
#include "fingerprinting/core/device_descriptor/fingerprint.h"
#include "fingerprinting/manager/manager.h"
#include "fingerprinting/manager/manager.mojom.h"
#include "mojo/public/cpp/bindings/pending_receiver.h"
#include "mojo/public/cpp/bindings/receiver.h"  // Add this line
#include "mojo/public/cpp/bindings/receiver_set.h"
#include "services/service_manager/public/cpp/service.h"
#include "services/service_manager/public/cpp/binder_registry.h"
#include "third_party/abseil-cpp/absl/types/optional.h"
#include "sandbox/policy/mojom/sandbox.mojom.h"
#include "base/command_line.h"
#include "mojo/public/cpp/bindings/self_owned_receiver.h"

namespace fingerprinting{
	class FingerprintManagerService
			: public service_manager::Service,
				public fingerprinting::mojom::FingerprintingManager {
	public:
		FingerprintManagerService()
				: fingerprint_manager_(
							fingerprinting::FingerprintManager::GetInstance()) {}
		
		// Implementation of fingerprinting::mojom::FingerprintingManager
		void GetFingerprintStr(GetFingerprintStrCallback callback) override {
			// Delegates to the underlying FingerprintManager
			return fingerprint_manager_->GetFingerprintStr(std::move(callback));
		}

		void SetFingerprintStr(const std::string& fingerprint_str,
													SetFingerprintStrCallback callback) override {
			// Delegates to the underlying FingerprintManager
			fingerprint_manager_->SetFingerprintStr(fingerprint_str, std::move(callback));
		}

		void Enabled(EnabledCallback callback) override {
			// Delegates to the underlying FingerprintManager
			return fingerprint_manager_->Enabled(std::move(callback));
		}

		void BindReceiver(
				mojo::PendingReceiver<fingerprinting::mojom::FingerprintingManager>
					receiver) {
			receiver_.Bind(std::move(receiver));
		}

		// void OnStart() override {
		// 	registry_.AddInterface(
		// 			base::BindRepeating(&FingerprintManagerService::BindFingerprintManager,
		// 													base::Unretained(this)));
		// }
		static std::unique_ptr<FingerprintManagerService> Create(mojo::PendingReceiver<fingerprinting::mojom::FingerprintingManager> receiver) {
			auto service = std::make_unique<FingerprintManagerService>();

			LOG(INFO) << "FingerprintManagerService::Create";

			fingerprinting::FingerprintManager* instance = fingerprinting::FingerprintManager::GetInstance();
			if (instance->CanBeInitialized()) {
				LOG(INFO) << "FingerprintManagerService::CanBeInitialized";
				if (base::CommandLine::ForCurrentProcess()->HasSwitch("fingerprint")) {
					LOG(INFO) << "HasSwitch";
					std::string fingerprint_file_path = base::CommandLine::ForCurrentProcess()->GetSwitchValueASCII("fingerprint");
					instance->Init(fingerprint_file_path);
				} else {
					LOG(INFO) << "NoSwitch";
					instance->SetCanBeInitialized(false);
				}
			}

			service->BindReceiver(std::move(receiver));
			return service;
		}


	  
	static const sandbox::mojom::Sandbox kServiceSandbox = sandbox::mojom::Sandbox::kRenderer;


	private:
		raw_ptr<fingerprinting::FingerprintManager> fingerprint_manager_;  // not owned
		//service_manager::BinderRegistry registry_;
		mojo::Receiver<fingerprinting::mojom::FingerprintingManager> receiver_{this};

		

	};

}
#endif
```


To then register this service, we have been advised to do so under content/utility/services.cc:


```
...
auto RunFingerprintingManagerService(mojo::PendingReceiver<fingerprinting::mojom::FingerprintingManager> receiver) {
  return fingerprinting::FingerprintManagerService::Create(std::move(receiver));
}
...

void RegisterMainThreadServices(services){
    ...
    services.Add(RunFingerprintingManagerService);
}
```


That's as far as goes our implementation.
For the consumption of the service itself, we do that under:
```
void RenderFrameImpl::DidCreateScriptContext(v8::Local<v8::Context> context,
                                             int world_id) {


  mojo::Remote<fingerprinting::mojom::FingerprintingManager> fingerprint_manager;
  GetBrowserInterfaceBroker()->GetInterface(
      fingerprint_manager.BindNewPipeAndPassReceiver());

  // Use the fingerprint_manager to call GetFingerprintStr
  std::string fingerprint_str;

  bool enabled = false;
  fingerprint_manager->Enabled(&enabled);
  if (enabled){

    fingerprint_manager->GetFingerprintStr(&fingerprint_str);

    if (fingerprint_str.empty()) {
      // Fingerprinting is blocked, so we should not enable the API.
      return;
    }
  }
  ...
```


The build passed, and we tried to test it with the debugger, however The FingerprintManagerService never gets initialized. We can assume that because neither logs or breakpoints are hitting, for example, fingerprinting::FingerprintManagerService::Create has a LOG on the first line, and it is never printed. 

What is wrong with the implementation?
