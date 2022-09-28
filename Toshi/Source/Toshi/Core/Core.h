#pragma once

constexpr uint32_t TMAKEFOUR(const char str[4])
{
	return ((str[3] << 24) | (str[2] << 16) | (str[1] << 8) | str[0]);
}

#define TMAKEVERSION(VER_MAJOR, VER_MINOR) ((VER_MAJOR << 16) | VER_MINOR)
#define BITFIELD(x) (1 << x)

#define _TS16(x) L#x

constexpr uint16_t BIG_ENDIAN_BYTES_TO_UINT16(const unsigned char bytes[2])
{
	return bytes[1] | bytes[0] << 8;
}

constexpr uint16_t BIG_ENDIAN_TO_UINT16(const unsigned short val)
{
	return ((((val) >> 8) & 0xff) | (((val) & 0xff) << 8));
}

#if defined(TOSHI_DEBUG)
	#if defined(TOSHI_PLATFORM_WINDOWS)
		#define TBREAK() __debugbreak()
		#define TOSHI_ENABLE_ASSERTS
	#endif
#endif

#ifndef TBREAK
	#define TBREAK() 
#endif

#ifndef TOSHI_PLATFORM_WINDOWS
	#error Unsupported platform
#endif

#if defined(TOSHI_ENABLE_ASSERTS)
	#define TTODO(DESC) { TOSHI_ERROR("TODO: {0} ({1}, at line {2})", DESC, __FUNCTION__, __LINE__); }
	#define TIMPLEMENT() { TOSHI_ERROR("{0} is not implemented", __FUNCTION__); }
	#define TASSERT(x, ...) { if (!(x)) { TOSHI_CORE_ERROR(__VA_ARGS__); TBREAK(); } }
#else
	#define TTODO(DESC)
	#define TIMPLEMENT()
	#define TASSERT(x, ...)
#endif