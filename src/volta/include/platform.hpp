/*
============================================================================
File: platform.hpp
Description:
    This file defines platform-dependend operations.
    Important:
    Here all functions are inline-defined.
Notes:
    These functions are implemented in assembly.
============================================================================
*/
#ifndef PLATFORM_HPP
#define PLATFORM_HPP

#include "types.hpp"
#include <x86intrin.h>

namespace Volta::Platform
{
    /*
    inline void flushTLB();
    inline void flushTLBEntry(Address vaddr);
    inline void switchPageTableContext(Address addr);

    inline void enableInterrupts();
    inline void disableInterrupts();
    */

    inline void halt();
    inline auto getMonotonicClock() -> uint64_t;

    #ifdef ARCH_x86_64
    inline void halt()
    {
        __asm__ __volatile__("hlt");
    }

    inline auto getMonotonicClock() -> uint64_t
    {
        unsigned int dummy;
        uint64_t tsc = __rdtscp(&dummy);
        _mm_lfence();
        return dummy;
    }
    #endif
}
#endif // PLATFORM_HPP