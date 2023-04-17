#pragma once

// TODO: add a checkf macro
// TODO: add an unreachable macro

#ifndef NO_CHECKS

#include <intrin.h>
#include <core/logger.h>

#define STRINGIFY(x) #x
#define TO_STR(x) STRINGIFY(x)

#define check(expression)                                                                        \
    do                                                                                           \
    {                                                                                            \
        if (!(expression)) [[unlikely]]                                                          \
        {                                                                                        \
            Logger::error("Assertion failed: " __FILE__ "(" TO_STR(__LINE__) "): " #expression); \
            __debugbreak();                                                                      \
        }                                                                                        \
    }                                                                                            \
    while (0)

#define verify(expression) check(expression)

#else

#define check(expression) ((void)0)
#define verify(expression) expression

#endif