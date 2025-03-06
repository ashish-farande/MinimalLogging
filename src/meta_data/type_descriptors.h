#ifndef AF1129B8_1BB3_4792_B59B_57DEC5152C58
#define AF1129B8_1BB3_4792_B59B_57DEC5152C58
#ifndef A2AE456B_683F_49D9_87F3_C98F503E0DAF
#define A2AE456B_683F_49D9_87F3_C98F503E0DAF

#include <stdio.h>
#include <variant>
#include <array>

#include "meta_data/span.h"
#include "meta_data/types.h"

using TypeDescriptor = std::variant<Int, Float, CStr>;
using TypeDescriptors = Span<TypeDescriptor>;

template <typename T>
struct GetTypeDescriptor;

template <>
struct GetTypeDescriptor<int>
{
    static constexpr TypeDescriptor value{Int{}};
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

#endif /* A2AE456B_683F_49D9_87F3_C98F503E0DAF */

#endif /* AF1129B8_1BB3_4792_B59B_57DEC5152C58 */
