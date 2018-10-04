#pragma once
#include "expander.h"
#include "lib/lock/Lock.h"


/**
 * A lock implementation using a Win32 CRITICAL_SECTION structure.
 */
class CriticalSection : public Lock {

   private: CRITICAL_SECTION m_cs;

   public:     CriticalSection();
      virtual ~CriticalSection();

      virtual void lock();
      virtual void unlock();
};
