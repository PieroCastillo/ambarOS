#ifndef AMBAROS_KERNEL_CPU_HPP
#define AMBAROS_KERNEL_CPU_HPP

/*
Kernel internal functions
*/

#include "types.hpp"

namespace AmbarOS::Volta::Internal {
    extern "C" void setCR3();

    extern "C" void vStoreRegisters();
    extern "C" void vFlushRegisters();
    extern "C" void vSwitchToUserSpace();
}

#endif