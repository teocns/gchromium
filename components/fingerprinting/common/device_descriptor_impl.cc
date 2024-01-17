#include <vector>
#include "base/json/json_reader.h"
#include "base/logging.h"

#include "components/fingerprinting/common/device_descriptor_impl.h"

namespace fingerprinting {

DeviceDescriptor::DeviceDescriptor(DeviceDescriptor&& other) = default;
DeviceDescriptor& DeviceDescriptor::operator=(DeviceDescriptor&& other) {
  if (this != &other) {
    IDeviceDescriptor::operator=(std::move(other));  // Move-assign the virtual base
    // Move-assign other members here, e.g.,
    value_ = std::move(other.value_);
    cache_ = std::move(other.cache_);
  }
  return *this;
}
// Destructor
DeviceDescriptor::~DeviceDescriptor() = default;
DeviceDescriptor::DeviceDescriptor(base::Value& value) {
  value_ = std::move(value);
}

DeviceDescriptor::DeviceDescriptor(base::Value&& value) {
  value_ = std::move(value);
}
bool DeviceDescriptor::Find(std::vector<std::string>&& keys) {
  /** Only checks for existency */
  base::Value* out = nullptr;
  return Find(std::move(keys), out);
}

bool DeviceDescriptor::Find(std::vector<std::string>&& keys, base::Value*& out) {
  /**
   * Utility to search for nested JSON values in the DeviceDescriptor dictionary.
   * Usage:
   *
   * base::Value* out = nullptr;
   */
  std::size_t hash = hash_keys(keys);

  if (find_in_cache(hash, out)) {
    LOG(ERROR) << "Got from cache";
    return true;
  }

  LOG(ERROR) << "Not in cache";

  // Iterate through the dictionary tree
  base::Value* cursor = &value_;
  if (!find_(keys, cursor)) {
    return false;
  }
  cache_.emplace(std::make_pair(hash, cursor));
  out = cursor;
  return true;
}

std::size_t DeviceDescriptor::hash_keys(std::vector<std::string>& keys) {
  std::size_t hash = 0;
  for (const auto& key : keys) {
    hash ^=
        std::hash<std::string>{}(key) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
  }
  return hash;
}

bool DeviceDescriptor::find_in_cache(const std::size_t& hash, base::Value*& out) {
  auto it = cache_.find(hash);
  if (it != cache_.end()) {
    // out should point to it->second's address
    out = it->second;
    return true;
  }
  return false;
}

bool DeviceDescriptor::find_(std::vector<std::string>& keys, base::Value*& cur) {
  if (cur == nullptr) {
    return false;
  }

  if (keys.empty()) {
    return true;
  }

  std::string key = keys.front();
  keys.erase(keys.begin());

  if (!cur->is_dict()) {
    // We have found a non-dictionary object in the tree, hence we can't
    // iterate over it.
    cur = nullptr;
    return false;
  }

  base::Value::Dict& dict = cur->GetDict();

  cur = dict.Find(key);  // Change to appropriate find method

  return find_(keys, cur);
}

bool DeviceDescriptor::FromString(const std::string& dd_str,
                             std::unique_ptr<DeviceDescriptor>& out) {
  absl::optional<base::Value> fp_maybe =
      base::JSONReader::Read(dd_str);

  if (!fp_maybe.has_value()) {
    return false;
  }

  base::Value& fp_val = fp_maybe.value();

  out = std::make_unique<DeviceDescriptor>(std::move(fp_val));
  out->str_value_ = dd_str;

  return true;
}
}  // namespace fingerprinting
