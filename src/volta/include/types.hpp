#ifndef TYPES_HPP
#define TYPES_HPP

#include <stdint.h>

using Process = unsigned long;
using Thread = unsigned long;
using Mutex = unsigned long;
using File = unsigned long;
using Address = void*;

enum Result
{
    Success,
    Error,
    OutOfMemory,
};

struct VString {
    const char* data;
    size_t size;
};

#endif //TYPES_HPP