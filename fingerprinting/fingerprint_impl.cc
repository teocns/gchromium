#include <vector>
#include "base/json/json_reader.h"
#include "base/logging.h"

#include "fingerprinting/fingerprint_impl.h"

namespace fingerprinting {

Fingerprint::Fingerprint(Fingerprint&& other) = default;
Fingerprint& Fingerprint::operator=(Fingerprint&& other) {
  if (this != &other) {
    IFingerprint::operator=(std::move(other));  // Move-assign the virtual base
    // Move-assign other members here, e.g.,
    value_ = std::move(other.value_);
    cache_ = std::move(other.cache_);
  }
  return *this;
}
// Destructor
Fingerprint::~Fingerprint() = default;
Fingerprint::Fingerprint(Val& value) {
  value_ = std::move(value);
}

Fingerprint::Fingerprint(Val&& value) {
  value_ = std::move(value);
}
bool Fingerprint::Find(std::vector<std::string>&& keys) {
  /** Only checks for existency */
  Val* out = nullptr;
  return Find(std::move(keys), out);
}

bool Fingerprint::Find(std::vector<std::string>&& keys, Val*& out) {
  /**
   * Utility to search for nested JSON values in the Fingerprint dictionary.
   * Usage:
   *
   * Val* out = nullptr;
   */
  std::size_t hash = hash_keys(keys);

  if (find_in_cache(hash, out)) {
    LOG(ERROR) << "Got from cache";
    return true;
  }

  LOG(ERROR) << "Not in cache";

  // Iterate through the dictionary tree
  Val* cursor = &value_;
  if (!find_(keys, cursor)) {
    return false;
  }
  cache_.emplace(std::make_pair(hash, cursor));
  out = cursor;
  return true;
}

std::size_t Fingerprint::hash_keys(std::vector<std::string>& keys) {
  std::size_t hash = 0;
  for (const auto& key : keys) {
    hash ^=
        std::hash<std::string>{}(key) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
  }
  return hash;
}

bool Fingerprint::find_in_cache(const std::size_t& hash, Val*& out) {
  auto it = cache_.find(hash);
  if (it != cache_.end()) {
    // out should point to it->second's address
    out = it->second;
    return true;
  }
  return false;
}

bool Fingerprint::find_(std::vector<std::string>& keys, Val*& cur) {
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

  Val::Dict& dict = cur->GetDict();

  cur = dict.Find(key);  // Change to appropriate find method

  return find_(keys, cur);
}

bool Fingerprint::FromString(const std::string& fingerprint_str,
                             std::unique_ptr<Fingerprint>& out) {
  absl::optional<Val> fp_maybe = base::JSONReader::Read(fingerprint_str);

  if (!fp_maybe.has_value()) {
    return false;
  }

  Val& fp_val = fp_maybe.value();

  out = std::make_unique<Fingerprint>(std::move(fp_val));

  return true;
}
}  // namespace fingerprinting
