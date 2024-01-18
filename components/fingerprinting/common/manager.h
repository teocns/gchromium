// fingerprint_manager.h
#ifndef FINGERPRINTING_MANAGER_CORE_H
#define FINGERPRINTING_MANAGER_CORE_H

#include <any>
#include <format>
#include <mutex>
#include <string>
#include <unordered_map>
#include "base/component_export.h"
#include "base/files/file_util.h"
#include "base/json/json_reader.h"
#include "base/logging.h"
#include "base/strings/string_util.h"
#include "base/values.h"
#include "components/fingerprinting/common/cache.h"
#include "components/fingerprinting/common/device_descriptor/fingerprint_impl.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

#include "base/command_line.h"
#include "base/debug/stack_trace.h"
#include "base/process/process_handle.h"

namespace fingerprinting::core {

class COMPONENT_EXPORT(FINGERPRINTING_CORE) FingerprintManagerCore {
 public:
  void Init();

  bool Loaded() { return loaded_; }

  static absl::optional<Fingerprint> FromStringToObj(
      const std::string& fingerprint_str);

  Fingerprint* GetFingerprint();

 protected:
  FingerprintManagerCore();
  ~FingerprintManagerCore();

  void LoadFingerprint(const std::string& fingerprint_file_path);
  bool CanBeInitialized() { return !initialized_ && can_be_initialized_; }
  Fingerprint* fingerprint() { return fingerprint_.get(); }

  static std::string get_debug_info(FingerprintManagerCore* manager,
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

  Cache cache;
 private:
  void SetCanBeInitialized(bool can_be) { can_be_initialized_ = can_be; }

  std::unique_ptr<Fingerprint> fingerprint_;

  base::Lock lock_;

  bool initialized_ =
      false;  // Initialization begun. Not yet a guarantee of loaded

  bool loaded_ = false;  // When the fingerprint is actually loaded

  // Usually set to false when switch isn't present
  bool can_be_initialized_ = true;



};

}  // namespace fingerprinting

#endif  // FINGERPRINTING_MANAGER_H_
