#pragma once
#include "expander.h"
#include "lib/lock/Lock.h"


/**
 * A lock implementation using a Win32 CRITICAL_SECTION structure.
 */
class CriticalSection : public Lock {

   /** The mutex used by this implementation */
   protected: CRITICAL_SECTION m_cs;


   /**
    * Constructor
    */
   public: CriticalSection() {
      InitializeCriticalSection(&m_cs);
   }


   /**
    * Destructor
    */
   public: ~CriticalSection() {
      DeleteCriticalSection(&m_cs);
   }


   /**
    * Lock the instance.
    */
   public: virtual void lock() {
      EnterCriticalSection(&m_cs);
   }


   /**
    * Unlock the instance.
    */
   public: virtual void unlock() {
      LeaveCriticalSection(&m_cs);
   }
};
