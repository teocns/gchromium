// Copyright 2011 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_SYNCHRONIZATION_RLOCK_H_
#define BASE_SYNCHRONIZATION_RLOCK_H_


#include "base/synchronization/lock_impl.h"
#include "base/threading/platform_thread.h"

namespace base{



class LOCKABLE BASE_EXPORT RLock {
 public:
  RLock();
  ~RLock();

  void Acquire() EXCLUSIVE_LOCK_FUNCTION();
  void Release() UNLOCK_FUNCTION();
  bool Try() EXCLUSIVE_TRYLOCK_FUNCTION(true);

  void AssertAcquired() const ASSERT_EXCLUSIVE_LOCK() {}
  void AssertNotHeld() const {}
  friend class ConditionVariable;
 private:
  base::internal::LockImpl lock_;
  base::PlatformThreadId owner_thread_id_;
  int recursion_count_;
};



using RAutoLock = internal::BasicAutoLock<RLock>;

}  // namespace base

#endif  // BASE_SYNCHRONIZATION_RLOCK_H_
