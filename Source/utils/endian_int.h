#pragma once

#include <type_traits>

namespace devilution {

template<typename T>
T SwapLE(T in)
{
	static_assert(std::is_integral_v<T> || std::is_enum_v<T>);
	static_assert((sizeof(T) == 1) || (sizeof(T) == 2) || (sizeof(T) == 4) || (sizeof(T) == 8));
	if constexpr (sizeof(T) == 1)
		return in;
	if constexpr (sizeof(T) == 2)
		return static_cast<T>(SDL_SwapLE16(static_cast<Uint16>(in)));
	if constexpr (sizeof(T) == 4)
		return static_cast<T>(SDL_SwapLE32(static_cast<Uint32>(in)));
	if constexpr (sizeof(T) == 8)
		return static_cast<T>(SDL_SwapLE64(static_cast<Uint64>(in)));
}

template<typename T>
T SwapBE(T in)
{
	static_assert(std::is_integral_v<T> || std::is_enum_v<T>);
	static_assert((sizeof(T) == 1) || (sizeof(T) == 2) || (sizeof(T) == 4) || (sizeof(T) == 8));
	if constexpr (sizeof(T) == 1)
		return in;
	if constexpr (sizeof(T) == 2)
		return static_cast<T>(SDL_SwapBE16(static_cast<Uint16>(in)));
	if constexpr (sizeof(T) == 4)
		return static_cast<T>(SDL_SwapBE32(static_cast<Uint32>(in)));
	if constexpr (sizeof(T) == 8)
		return static_cast<T>(SDL_SwapBE64(static_cast<Uint64>(in)));
}

template <typename T, T (*f)(T)>
class endian_int {
	T value;

public:
	endian_int() :
		value(0)
	{
	}

	endian_int(const endian_int<T, f>& in) :
		value(in.value)
	{
	}

	endian_int(const T& in) :
		value(f(in))
	{
	}

	endian_int<T, f>& operator=(const endian_int<T, f>& in)
	{
		value = in.value;
		return *this;
	}

	endian_int<T, f>& operator=(const T& in)
	{
		value = f(in);
		return *this;
	}

	endian_int<T, f>& operator-=(const T& in)
	{
		value = f(f(value) - in);
		return *this;
	}

	endian_int<T, f>& operator+=(const T& in)
	{
		value = f(f(value) + in);
		return *this;
	}

	operator T() const
	{
		return f(value);
	}
};

template<typename N, typename T, T (*f)(T)>
N endian_cast(const endian_int<T, f>& in)
{
	return static_cast<N>(static_cast<T>(in));
}

using Uint16_BE = endian_int<Uint16, SwapBE>;
using Uint32_BE = endian_int<Uint32, SwapBE>;
using Uint64_BE = endian_int<Uint64, SwapBE>;
using Sint16_BE = endian_int<Sint16, SwapBE>;
using Sint32_BE = endian_int<Sint32, SwapBE>;
using Sint64_BE = endian_int<Sint64, SwapBE>;

using Uint16_LE = endian_int<Uint16, SwapLE>;
using Uint32_LE = endian_int<Uint32, SwapLE>;
using Uint64_LE = endian_int<Uint64, SwapLE>;
using Sint16_LE = endian_int<Sint16, SwapLE>;
using Sint32_LE = endian_int<Sint32, SwapLE>;
using Sint64_LE = endian_int<Sint64, SwapLE>;

using Uint16_NET = Uint16_BE;
using Uint32_NET = Uint32_BE;
using Uint64_NET = Uint64_BE;
using Sint16_NET = Sint16_BE;
using Sint32_NET = Sint32_BE;
using Sint64_NET = Sint64_BE;

}
