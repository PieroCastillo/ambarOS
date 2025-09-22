#ifndef SYSCALLS_HPP
#define SYSCALLS_HPP

#define SYSCALL

/*
system calls
*/

#include "types.hpp"

namespace AmbarOS::Volta {


    /*
    Processes
    Memory Pools
    Channels
    */

    // Threading
    auto SYSCALL CreateProcess(Process* process)->Result;

    auto SYSCALL RunProcess(Process* process)->Result;
    auto SYSCALL DestroyProcess(Process* process)->Result;

    // Memory management
    auto SYSCALL AllocateMemory(Address address)->Result;
    auto SYSCALL FreeMemory(Address address)->Result;

    // Channels
    auto SYSCALL CreateChannel() -> Result;
    auto SYSCALL DestroyChannel() -> Result;

}

#endif // SYSCALLS_HPP