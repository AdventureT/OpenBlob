#pragma once

namespace Toshi
{
	class TSystem
	{
	public:
		static size_t StringLength(const char* const& str);
		static size_t StringLength(const wchar_t* const& wstr);
		static void* MemCopy(void* dst, const void* const& src, size_t size);
		static const char* StringUnicodeToChar(char*, wchar_t*, uint32_t);
	};
}


