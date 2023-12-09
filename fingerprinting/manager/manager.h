// fingerprint_manager.h
#ifndef FINGERPRINTING_MANAGER_IFACE_H_
#define FINGERPRINTING_MANAGER_IFACE_H_

#include <any>
#include <format>
#include <mutex>
#include <string>
#include <unordered_map>
#include "base/files/file_util.h"
#include "base/json/json_reader.h"
#include "base/logging.h"
#include "base/no_destructor.h"
#include "base/strings/string_util.h"
#include "base/synchronization/rlock.h"
#include "base/values.h"
#include "fingerprinting/export.h"
#include "fingerprinting/fingerprint_impl.h"
#include "fingerprinting/manager.mojom.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

#include "base/command_line.h"
#include "base/debug/stack_trace.h"
#include "base/process/process_handle.h"

namespace fingerprinting {

class FINGERPRINTING_EXPORT IFingerprintManager
    : virtual public fingerprinting::mojom::FingerprintManager {
 public:
  void Init();

  bool Loaded() { return loaded_; }

  static absl::optional<Fingerprint> FromStringToObj(
      const std::string& fingerprint_str);

  Fingerprint* GetFingerprint();

 protected:
  IFingerprintManager();

  ~IFingerprintManager() override;
  void LoadFingerprint(const std::string& fingerprint_file_path);
  bool CanBeInitialized() { return !initialized_ && can_be_initialized_; }
  Fingerprint* fingerprint() { return fingerprint_.get(); }

  std::unordered_map<std::string, std::any> cache_;
  std::unordered_map<std::string, std::mutex> key_mutexes_;
  std::mutex
      key_mutexes_map_mutex_;  // To protect access to key_mutexes_ itself

  // Definition of the CacheValue template function
  template <typename T>
  void CacheValue(const std::string& key, const std::shared_ptr<T>& value) {
    std::unique_lock<std::mutex> map_lock(key_mutexes_map_mutex_);
    auto& key_mutex = key_mutexes_[key];
    map_lock.unlock();  // unlock map mutex as soon as possible

    std::unique_lock<std::mutex> key_lock(key_mutex);  // lock for specific key
    cache_[key] = value;
  }

  template <typename T>
  bool GetFromCache(const std::string& key, std::shared_ptr<T>& value) {
    std::unique_lock<std::mutex> map_lock(key_mutexes_map_mutex_);
    auto it = key_mutexes_.find(key);
    if (it == key_mutexes_.end()) {
      // Key not found in the mutex map; maybe it should be created here?
      return false;
    }
    std::mutex& key_mutex = it->second;
    map_lock.unlock();  // unlock as soon as possible

    std::unique_lock<std::mutex> key_lock(key_mutex);  // lock for specific key
    auto cache_it = cache_.find(key);
    if (cache_it != cache_.end()) {
      value = std::dynamic_pointer_cast<T>(cache_it->second);
      return true;
    }
    return false;  // Key not found in cache
  }

  static std::string get_debug_info(IFingerprintManager* manager,
                                    bool with_stack_trace = true) {
    // std::string process_type =
    //     base::CommandLine::ForCurrentProcess()->GetSwitchValueASCII("type");

    base::ProcessId pid = base::GetCurrentProcId();

    Fingerprint* fp = manager->GetFingerprint();

    std::string fp_mem_addr =
        std::format("0x{:x}", reinterpret_cast<uintptr_t>(fp));

    std::string stack;

    if (with_stack_trace) {
      stack = get_stack_trace();
    }

    return std::format("PID: {} | PTR: {} | loaded {} | mojom: {}",
                       std::move(pid), std::move(fp_mem_addr),
                       manager->Loaded(), is_mojom(stack));
  }

  static std::string get_stack_trace(int n = 15) {
    base::debug::StackTrace stack_trace(n);
    return stack_trace.ToString();
  }

  static bool is_mojom(const std::string& callstack) {
    constexpr char kPendingReceiver[] = "PendingReceiver";
    if (callstack.find(kPendingReceiver) != std::string::npos) {
      return true;
    }
    return false;
  }

 private:
  void SetCanBeInitialized(bool can_be) { can_be_initialized_ = can_be; }

  std::unique_ptr<Fingerprint> fingerprint_;

  base::RLock rlock_;
  base::Lock lock_;

  bool initialized_ =
      false;  // Initialization begun. Not yet a guarantee of loaded

  bool loaded_ = false;  // When the fingerprint is actually loaded

  // Usually set to false when switch isn't present
  bool can_be_initialized_ = true;
};

}  // namespace fingerprinting

#endif  // FINGERPRINTING_MANAGER_H_
