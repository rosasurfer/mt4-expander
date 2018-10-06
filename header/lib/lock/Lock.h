#pragma once
#include "expander.h"
#include <map>


/**
 * Abstract base class for lock implementations.
 */
class Lock {

   /**
    * Constructor
    */
   protected: Lock() {
   };

   /**
    * Destructor
    */
   public: virtual ~Lock() {
   };

   public:
      virtual void lock()   abstract;
      virtual void unlock() abstract;
};


typedef std::map<char*, Lock*> Locks;                                // a map holding pointers to fine-granular locks
