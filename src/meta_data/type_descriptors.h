#ifndef AF1129B8_1BB3_4792_B59B_57DEC5152C58
#define AF1129B8_1BB3_4792_B59B_57DEC5152C58

#include <stdio.h>
#include <variant>
#include <array>

#include "utils/span.h"
#include "meta_data/types.h"

namespace meta_data
{
    using TypeDescriptor = std::variant<Bool, Char, UnsignedChar, ShortInt, UnsignedShortInt, Int,
                                        UnsignedInt, LongInt, UnsignedLongInt, LongLongInt, UnsignedLongLongInt,
                                        Double, LongDouble, Float, CStr>;
    using TypeDescriptors = Span<TypeDescriptor>;

    template <typename T>
    struct GetTypeDescriptor;

    template <>
    struct GetTypeDescriptor<bool>
    {
        static constexpr TypeDescriptor value{Bool{}};
    };

    template <>
    struct GetTypeDescriptor<char>
    {
        static constexpr TypeDescriptor value{Char{}};
    };

    template <>
    struct GetTypeDescriptor<unsigned char>
    {
        static constexpr TypeDescriptor value{UnsignedChar{}};
    };

    template <>
    struct GetTypeDescriptor<short int>
    {
        static constexpr TypeDescriptor value{ShortInt{}};
    };

    template <>
    struct GetTypeDescriptor<unsigned short int>
    {
        static constexpr TypeDescriptor value{UnsignedShortInt{}};
    };

    template <>
    struct GetTypeDescriptor<int>
    {
        static constexpr TypeDescriptor value{Int{}};
    };

    template <>
    struct GetTypeDescriptor<unsigned int>
    {
        static constexpr TypeDescriptor value{UnsignedInt{}};
    };

    template <>
    struct GetTypeDescriptor<long int>
    {
        static constexpr TypeDescriptor value{LongInt{}};
    };

    template <>
    struct GetTypeDescriptor<unsigned long int>
    {
        static constexpr TypeDescriptor value{UnsignedLongInt{}};
    };

    template <>
    struct GetTypeDescriptor<long long int>
    {
        static constexpr TypeDescriptor value{LongLongInt{}};
    };

    template <>
    struct GetTypeDescriptor<unsigned long long int>
    {
        static constexpr TypeDescriptor value{UnsignedLongLongInt{}};
    };

    template <>
    struct GetTypeDescriptor<double>
    {
        static constexpr TypeDescriptor value{Double{}};
    };

    template <>
    struct GetTypeDescriptor<long double>
    {
        static constexpr TypeDescriptor value{LongDouble{}};
    };

    template <>
    struct GetTypeDescriptor<float>
    {
        static constexpr TypeDescriptor value{Float{}};
    };

    template <>
    struct GetTypeDescriptor<const char *>
    {
        static constexpr TypeDescriptor value{CStr{}};
    };

    template <>
    struct GetTypeDescriptor<std::string>
    {
        static constexpr TypeDescriptor value{CStr{}};
    };
}

#endif /* AF1129B8_1BB3_4792_B59B_57DEC5152C58 */
