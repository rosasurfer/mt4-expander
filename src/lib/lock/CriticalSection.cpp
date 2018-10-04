#include "expander.h"
#include "lib/lock/CriticalSection.h"


/**
 * Constructor
 */
CriticalSection::CriticalSection() {
   InitializeCriticalSection(&m_cs);
}


/**
 * Destructor
 */
CriticalSection::~CriticalSection() {
   DeleteCriticalSection(&m_cs);
}


/**
 * Lock the instance.
 */
void CriticalSection::lock() {
   EnterCriticalSection(&m_cs);
}


/**
 * Unlock the instance.
 */
void CriticalSection::unlock() {
   LeaveCriticalSection(&m_cs);
}
