#ifndef FINGERPRINTING_CORE_EVASIONS_HOOK_REGISTRY
#define FINGERPRINTING_CORE_EVASIONS_HOOK_REGISTRY

#include <algorithm>
#include <functional>
#include <memory>
#include "base/component_export.h"
#include "components/fingerprinting/renderer/evasions/hook.h"

namespace fingerprinting::core::evasions {

typedef std::function<std::unique_ptr<Hook>()> HookConstructor;

struct HookRegistryEntry {
  HookRegistryEntry(const std::string& key,
                    int priority,
                    HookConstructor constructor)
      : key(key), priority(priority), value(constructor) {}

  // Make it copiable
  HookRegistryEntry(const HookRegistryEntry& other) {
    key = other.key;
    priority = other.priority;
    value = other.value;
  }

  std::string key;
  int priority;
  HookConstructor value;

  // LLT operator
  bool operator<(const HookRegistryEntry& other) const {
    return priority < other.priority;
  }
};

class HookRegistry {
 public:
  void Register(const std::string& key,
                int priority,
                HookConstructor constructor) {
    HookRegistryEntry entry(key, priority, constructor);
    reg_priority_.emplace(priority, entry);
    reg_key_.emplace(key, entry);
  }

  HookRegistryEntry* Get(const std::string& key) {
    auto it = reg_key_.find(key);
    if (it != reg_key_.end()) {
      return &it->second;
    }
    return nullptr;
  }

  // ------------------------------------
  // ITERATOR
  // Define iterator type using the iterator of the std::multimap
  using iterator = std::multimap<int, HookRegistryEntry>::iterator;
  using const_iterator = std::multimap<int, HookRegistryEntry>::const_iterator;

  // begin and end methods to enable range-based for loops
  iterator begin() { return reg_priority_.begin(); }

  iterator end() { return reg_priority_.end(); }

  const_iterator begin() const { return reg_priority_.cbegin(); }

  const_iterator end() const { return reg_priority_.cend(); }

 private:
  std::multimap<int, HookRegistryEntry> reg_priority_;
  std::unordered_map<std::string, HookRegistryEntry> reg_key_;
};

}  // namespace fingerprinting::core::evasions

#endif  // FINGERPRINTING_CORE_EVASIONS_HOOK_REGISTRY
