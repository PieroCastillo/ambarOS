/*
============================================================================
File: kernel.hpp
Description:
    This file contains types used by the Volta kernel.
============================================================================
*/
#ifndef KERNEL_HPP
#define KERNEL_HPP

#include "types.hpp"
#include "platform.hpp"

#define ambarOSfn extern "C"

namespace Volta
{
    using PhysicalAddress = uint64_t;
    using VirtualAddress = uint64_t;

    enum class PageFlags : uint8_t
    {
        None,
    };

    auto allocPages(uint64_t count) -> PhysicalAddress;
    void freePages(PhysicalAddress pAddr, uint64_t count);

    auto reserve(uint64_t count) -> VirtualAddress;
    void release(VirtualAddress vAddr, uint64_t count);

    void map(VirtualAddress vAddr, PhysicalAddress pAddr, PageFlags flags);
    void unmap(VirtualAddress vAddr);
}
#endif // KERNEL_HPP