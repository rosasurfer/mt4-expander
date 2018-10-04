#pragma once
#include "expander.h"


/**
 * Abstract base class for lock implementations.
 */
class Lock {

   protected:  Lock() {};
   public:
      virtual ~Lock() {};

      virtual void lock()   = 0;
      virtual void unlock() = 0;
};
