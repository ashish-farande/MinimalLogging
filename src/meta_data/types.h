#ifndef E41F9D0F_BF16_4454_BC7F_EB7F76BAC9C5
#define E41F9D0F_BF16_4454_BC7F_EB7F76BAC9C5

#include <variant>

using LogDataTypes = std::variant<
    int, float, double, long double, const char *,
    bool, char, unsigned char,
    short int, unsigned short int,
    unsigned int,
    long int, unsigned long int,
    long long int, unsigned long long int>;

struct Bool
{
};

struct Char
{
};

struct UnsignedChar
{
};

struct ShortInt
{
};

struct UnsignedShortInt
{
};

struct Int
{
};

struct UnsignedInt
{
};

struct LongInt
{
};

struct UnsignedLongInt
{
};

struct LongLongInt
{
};

struct UnsignedLongLongInt
{
};

struct Float
{
};

struct Double
{
};

struct LongDouble
{
};

struct CStr
{
};

#endif /* E41F9D0F_BF16_4454_BC7F_EB7F76BAC9C5 */
