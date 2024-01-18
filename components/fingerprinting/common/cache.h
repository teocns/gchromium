
// fingerprint_manager.h
#ifndef FINGERPRINTING_CORE_CACHE_H
#define FINGERPRINTING_CORE_CACHE_H

#include <any>
#include <format>
#include <mutex>
#include <string>
#include <unordered_map>
#include "base/component_export.h"
#include "base/files/file_util.h"
#include "base/json/json_reader.h"
#include "base/logging.h"
#include "base/no_destructor.h"
#include "base/strings/string_util.h"
#include "base/values.h"
#include "components/fingerprinting/common/device_descriptor/fingerprint_impl.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

#include "base/command_line.h"
#include "base/debug/stack_trace.h"
#include "base/process/process_handle.h"

namespace fingerprinting::core {

class COMPONENT_EXPORT(FINGERPRINTING_CORE) Cache {
 private:
  std::unordered_map<std::string, std::any> cache_;
  std::unordered_map<std::string, std::mutex> key_mutexes_;
  std::mutex
      key_mutexes_map_mutex_;  // To protect access to key_mutexes_ itself
 public:
  /*
    It is ok to accept a reference, because std::any stores an internal copy
    given the std::any type
  */
  template <typename T>
  void Set(const std::string& key, T& value) {
    std::unique_lock<std::mutex> map_lock(key_mutexes_map_mutex_);
    auto& key_mutex = key_mutexes_[key];
    map_lock.unlock();  // unlock map mutex as soon as possible

    std::unique_lock<std::mutex> key_lock(key_mutex);  // lock for specific key

    cache_[key] = value;
  }
  template <typename T>
  bool Set(const std::string& key, std::shared_ptr<T> value) {
    // Overload for shared pointers
    std::unique_lock<std::mutex> map_lock(key_mutexes_map_mutex_);
    auto& key_mutex = key_mutexes_[key];
    map_lock.unlock();  // unlock map mutex as soon as possible

    std::unique_lock<std::mutex> key_lock(key_mutex);  // lock for specific key

    cache_[key] = value;
    return true;
  }
  template <typename T>
  void Set(const std::string& key, T&& value) {
    std::unique_lock<std::mutex> map_lock(key_mutexes_map_mutex_);
    auto& key_mutex = key_mutexes_[key];
    map_lock.unlock();  // unlock map mutex as soon as possible

    std::unique_lock<std::mutex> key_lock(key_mutex);  // lock for specific key

    cache_[key] = std::move(value);
  }
  template <typename T>
  bool Get(const std::string& key, T& value) {
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
      value = std::any_cast<T>(cache_[key]);
      return true;
    }
    return false;  // Key not found in cache
  }
  template <typename T>
  bool Get(const std::string& key, std::shared_ptr<T>& value) {
    // Overload for shared pointers
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
      value = std::any_cast<std::shared_ptr<T>>(cache_[key]);
      return true;
    }
    return false;  // Key not found in cache
  }
  template <typename T>
  bool Get(const std::string& key, absl::optional<T>& value) {
    // Overload for absl::optional
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
      value = std::any_cast<T>(cache_[key]);
      return true;
    }
    return false;  // Key not found in cache
  }

  template <typename T>
  bool Get(const std::string& key, T*& value) {
    // Overload for pointers
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
      value = std::any_cast<T*>(cache_[key]);
      return true;
    }
    return false;  // Key not found in cache
  }
};

}  // namespace fingerprinting::core
#endif  // FINGERPRINTING_CORE_CACHE_H
