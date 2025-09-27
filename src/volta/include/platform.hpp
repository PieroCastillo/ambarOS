/*
============================================================================
File: platform.hpp
Description:
    This file defines platform-dependend operations.
Notes:
    These functions are implemented in assembly.
============================================================================
*/
#ifndef PLATFORM_HPP
#define PLATFORM_HPP

#include "types.hpp"

namespace Volta::Platform
{
    /*
    inline void flushTLB();
    inline void flushTLBEntry(Address vaddr);
    inline void switchPageTableContext(Address addr);

    inline void enableInterrupts();
    inline void disableInterrupts();
    */
    inline void halt() 
    {
        __asm__ __volatile__("hlt");
    }
    /*
    inline void flushRegisters();
    */
}
#endif // PLATFORM_HPP