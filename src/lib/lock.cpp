#include "expander.h"
#include "lib/lock/CriticalSection.h"
#include "lib/lock/Lock.h"


Locks g_locks;                                              // a map holding pointers to fine-granular locks


/**
 * Get a lock (a mutex) for a specific code location.
 *
 * @param  char* file - code filename
 * @param  uint  line - code line number
 *
 * @return Lock*
 */
Lock* WINAPI GetLocalizedLock(char* file, uint line) {
   // generate the mapping key
   char* format = "%s#%p";
   size_t size = strlen(file) + 10;                         // file +"#FFFFFFFF"<NUL>: 1 + 8 + 1
   char* key = (char*) alloca(size);                        // on the stack
   sprintf_s(key, size, format, file, line);

   // find an existing lock or create and store a new one
   Lock* lock = NULL;
   if (g_locks.find(key) == g_locks.end()) lock = g_locks[key] = new CriticalSection();
   else                                    lock = g_locks[key];

   return(lock);
}


/**
 * Return the passed lock instance (a mutex).
 *
 * @param  char* file - ignored
 * @param  uint  line - ignored
 * @param  Lock& lock
 *
 * @return Lock*
 */
Lock* WINAPI GetLocalizedLock(char* file, uint line, Lock& lock) {
   return(&lock);
}
