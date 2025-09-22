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
    extern "C" void flushTLB();
    extern "C" void flushTLBEntry(Address vaddr);
    extern "C" void switchPageTableContext(Address addr);

    extern "C" void enableInterrupts();
    extern "C" void disableInterrupts();
    */
    extern "C" void halt();

    /*
    extern "C" void flushRegisters();
    */
}
#endif // PLATFORM_HPP