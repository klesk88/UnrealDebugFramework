// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// engine
#include "Templates/UnrealTypeTraits.h"

// std
#include <cstdint>
#include <type_traits>

#define GENERATE_STATIC_FUNCTION_CHECK(_FunctionName, _Result, ...) \
    template <typename T>                                           \
    class THasStaticFunction_##_FunctionName                        \
    {                                                               \
        template <typename U, _Result (*)(__VA_ARGS__)>             \
        struct Check;                                               \
        template <typename U>                                       \
        static char StaticTest(Check<U, &T::_FunctionName>*);       \
        template <typename U>                                       \
        static int32 StaticTest(...);                               \
                                                                    \
    public:                                                         \
        enum                                                        \
        {                                                           \
            Value = sizeof(StaticTest<T>(nullptr)) == sizeof(char)  \
        };                                                          \
    };
