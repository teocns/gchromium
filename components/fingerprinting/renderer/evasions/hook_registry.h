#ifndef FINGERPRINTING_CORE_EVASIONS_HOOK_REGISTRY
#define FINGERPRINTING_CORE_EVASIONS_HOOK_REGISTRY

#include <algorithm>
#include <functional>
#include <memory>
#include <set>
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

  bool operator<(const HookRegistryEntry& other) const {
    if (priority == other.priority) {
      return key < other.key;  // Tie-breaker if priorities are equal
    }
    return priority > other.priority;
  }
};

class HookRegistry {
 public:
  void Register(const std::string& key,
                int priority,
                HookConstructor constructor) {
    HookRegistryEntry entry(key, priority, constructor);
    entries_.insert(std::move(entry));
  }

  // Iterators
  auto begin() const { return entries_.begin(); }
  auto end() const { return entries_.end(); }

 private:
  // std::multimap<int, HookRegistryEntry> reg_priority_;
  // std::unordered_map<std::string, HookRegistryEntry> reg_key_;
  std::set<HookRegistryEntry> entries_;
};

// Let's moveo the iterator logic out of the HookRegistry

}  // namespace fingerprinting::core::evasions

#endif  // FINGERPRINTING_CORE_EVASIONS_HOOK_REGISTRY
