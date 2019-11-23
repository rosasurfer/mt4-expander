#pragma once
#include "expander.h"
#include "lib/lock/Lock.h"


/**
 * A class to aquire a lock on instance construction, and to relase it on instance destruction (i.e. a scoped lock).
 */
class Locker {

   /** The lock used by the instance. */
   protected: Lock &m_lock;


   /**
    * Constructor
    *
    * @param  Lock &lock - lock implementation
    */
   public: Locker(Lock &lock) : m_lock(lock) {
      debug("locking...");
      m_lock.lock();
   }

   /**
    * Destructor
    */
   public: virtual ~Locker() {
      debug("unlocking...");
      m_lock.unlock();
   }
};


// convenient helper to synchronize access to a scoped code block
#define synchronize(...)  Locker __lock__(*GetLocalizedLock(__FILE__, __LINE__, __VA_ARGS__));

Lock* WINAPI GetLocalizedLock(char* file, uint line);
Lock* WINAPI GetLocalizedLock(char* file, uint line, Lock &lock);
