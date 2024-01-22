#include <vector>
#include "base/json/json_reader.h"
#include "base/logging.h"
#include "components/fingerprinting/common/fingerprint_impl.h"

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
Fingerprint::Fingerprint(base::Value& value) {
  value_ = std::move(value);
}

Fingerprint::Fingerprint(base::Value&& value) {
  value_ = std::move(value);
}
bool Fingerprint::Find(std::vector<std::string>&& keys) {
  /** Only checks for existency */
  base::Value* out = nullptr;
  return Find(std::move(keys), out);
}

bool Fingerprint::Find(std::vector<std::string>&& keys, base::Value*& out) {
  /**
   * Utility to search for nested JSON values in the Fingerprint dictionary.
   * Usage:
   *
   * base::Value* out = nullptr;
   */
  std::size_t hash = hash_keys(keys);

  if (find_in_cache(hash, out)) {
    return true;
  }
  // Iterate through the dictionary tree
  base::Value* cursor = &value_;
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

bool Fingerprint::find_in_cache(const std::size_t& hash, base::Value*& out) {
  auto it = cache_.find(hash);
  if (it != cache_.end()) {
    // out should point to it->second's address
    out = it->second;
    return true;
  }
  return false;
}

bool Fingerprint::find_(std::vector<std::string>& keys, base::Value*& cur) {
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

absl::optional<Fingerprint> Fingerprint::From(
    const std::string& fingerprint_str) {
  absl::optional<Fingerprint> fp = absl::nullopt;
  auto parsed = base::JSONReader::Read(fingerprint_str);
  if (!parsed.has_value()) {
    return fp;
  }

  if (parsed != absl::nullopt) {
    fp = Fingerprint(std::move(parsed.value()));
  }
  return fp;
}
}  // namespace fingerprinting
