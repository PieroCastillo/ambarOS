#ifndef PLAT_CONSTANTS_HPP
#define PLAT_CONSTANTS_HPP

#include <stdint.h>

#if defined(__x86_64__)
inline constexpr uint32_t Page     = 4 * 1024;
inline constexpr uint32_t LargePage = 2 * 1024 * 1024;
inline constexpr uint32_t HugePage = 1 * 1024 * 1024 * 1024;

#elif defined(__aarch64__) || defined(__arm__)
inline constexpr uint32_t Page     =  4 * 1024;
inline constexpr uint32_t LargePage =  16 * 1024;
inline constexpr uint32_t HugePage =  64 * 1024;

#else
    #error "Not Supported Architecture"
#endif

#endif // PLAT_CONSTANTS_HPP