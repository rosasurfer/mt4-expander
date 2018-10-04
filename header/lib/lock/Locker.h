#pragma once
#include "expander.h"
#include "lib/lock/Lock.h"


/**
 * A class to aquire a lock on instance construction, and to relase it on instance destruction (i.e. a scoped lock).
 */
class Locker {

   private: Lock& m_lock;

   /**
    * @param  Lock& lock - lock implementation
    */
   public:     Locker(Lock& lock);
      virtual ~Locker();
};


// convenient helper to synchronize access to a scoped code block
#define synchronize(...)  Locker __lock(*GetLock(__FILE__, __LINE__, __VA_ARGS__));

Lock* WINAPI GetLock(char* file, uint line);
Lock* WINAPI GetLock(char* file, uint line, Lock& lock);
