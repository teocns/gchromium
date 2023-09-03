#include "base/synchronization/rlock.h"

namespace base{


RLock::RLock() : lock_(), owner_thread_id_(0), recursion_count_(0) {}

RLock::~RLock() {}

void RLock::Acquire() EXCLUSIVE_LOCK_FUNCTION (){
    base::PlatformThreadId current_thread_id = base::PlatformThread::CurrentId();
    lock_.Lock();
    if (owner_thread_id_ == current_thread_id) {
        recursion_count_++;
    } else {
        owner_thread_id_ = current_thread_id;
        recursion_count_ = 1;
    }
    lock_.Unlock();
}

void RLock::Release() UNLOCK_FUNCTION() {
    lock_.Lock();
    if (--recursion_count_ == 0) {
        owner_thread_id_ = 0;
    }
    lock_.Unlock();
}

bool RLock::Try() EXCLUSIVE_TRYLOCK_FUNCTION(true) {
    if (lock_.Try()) {
        base::PlatformThreadId current_thread_id = base::PlatformThread::CurrentId();
        if (owner_thread_id_ == current_thread_id) {
        recursion_count_++;
        } else {
        owner_thread_id_ = current_thread_id;
        recursion_count_ = 1;
        }
        return true;
    }
    return false;
    
}



} 
