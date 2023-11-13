
#ifndef _ENDIAN_HELPERS_H_
#define _ENDIAN_HELPERS_H_

#include <concepts>

template<typename T>
requires std::integral<T>
constexpr bool IsBigEndianRuntime()
{
    inline const union Temp{
        T i;
        char c[1];
    } temp = {0x01020304};

    return temp.c[0] == 1;
}

template<typename T>
requires std::integral<T>
inline void BigToNativeEndian(T& num)
{
    if constexpr (std::endian::native == std::endian::big)
    {
        // Already in native endian, do nothing
    }
    else if constexpr (std::endian::native == std::endian::little)
    {
        num = std::byteswap(num);
    }
    else
    {
        if (!IsBigEndianRuntime<T>())
        {
            num = std::byteswap(num);
        }
    }
}

template<typename T>
requires std::integral<T>
inline void NativeToBigEndian(T& num)
{
    BigToNativeEndian(num);
}

#endif