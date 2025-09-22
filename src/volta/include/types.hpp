/*
============================================================================
File: types.hpp
Description:
    This file contains types used by the Volta kernel.
============================================================================
*/
#ifndef TYPES_HPP
#define TYPES_HPP

#include <cstdint>

namespace Volta
{
    using Process = uint64_t;
    using Thread = uint64_t;
    using Mutex = uint64_t;
    using Address = uint64_t;

    enum ResultType
    {
        Success,
        Error,
    };
}
#endif // TYPES_HPP