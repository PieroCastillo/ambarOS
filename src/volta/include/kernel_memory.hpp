#ifndef AMBAROS_KERNEL_MEMORY_HPP
#define AMBAROS_KERNEL_MEMORY_HPP

/*
Kernel internal functions
*/

#include "types.hpp"

namespace AmbarOS::Volta::Internal {

    /* Pages Frames

    - Pages works with physical addresses

    Linear address : |  Dir  |  Table | Offset |
    */


    /// @brief Allocate a new page frame in physical memory
    /// @return Physical address of the page frame 
    auto vAllocPage() -> Address;
    auto vReleasePage(Address vAddress) -> void;

    struct Allocator
    {
        
    };
}

#endif