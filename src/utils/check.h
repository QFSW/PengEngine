#pragma once

// TODO: add a checkf macro
// TODO: add an unreachable macro

#ifndef NO_CHECKS

#include <core/logger.h>

#if defined(PLATFORM_WIN)

#include <intrin.h>
#define __break() __debugbreak()

#elif defined(PLATFORM_MAC)

#define __break() __builtin_debugtrap()

#else

#define __break() ((void)0)

#endif

#define STRINGIFY(x) #x
#define TO_STR(x) STRINGIFY(x)

#define check(expression)                                                                        \
    do                                                                                           \
    {                                                                                            \
        if (!(expression)) [[unlikely]]                                                          \
        {                                                                                        \
            Logger::error("Assertion failed: " __FILE__ "(" TO_STR(__LINE__) "): " #expression); \
            __break();                                                                           \
        }                                                                                        \
    }                                                                                            \
    while (0)

#define verify(expression) check(expression)

#else

#define check(expression) ((void)0)
#define verify(expression) expression

#endif