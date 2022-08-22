#include "pch.h"
#include "TCString.h"

#include "Toshi/Core/TSystem.h"

namespace Toshi
{
	TCString::TCString()
	{
		Reset();
		AllocBuffer(0, true);

		// this+7 = 0
		/*  if (param_1 == 0) {
		if ((_DAT_009b1704 == 0) && (sm_pDefaultAllocatorCB == (code *)0x0)) {
		  puVar1 = (undefined4 *)
				   Assert("TNULL!=sm_pDefaultAllocatorCB","..\\..\\Source\\Toshi\\TString8.cpp",100,
						  "TASSERT");
		  return puVar1;
		}
		param_1 = (*sm_pDefaultAllocatorCB)();
	  }
	  *(int *)((int)this + 8) = param_1;
	  uVar2 = (*(int *)((int)this + 4) << 8) >> 8;
	  if ((_DAT_009b1700 == 0) && (0x7fffffff < uVar2)) {
		puVar1 = (undefined4 *)
				 Assert("a_iX <= TINT32_MAX",
						"E:\\src\\Development\\ToshiBranch\\Source\\Math/TMathInline.h",0x67,"TASSERT");
		return puVar1;
	  }
	  if (uVar2 != 0) {
		(**(code**)(**(int**)((int)this + 8) + 8))(*this);
		*(undefined*)((int)this + 7) = 0;
		*(uint*)((int)this + 4) = *(uint*)((int)this + 4) & 0xff000000;
		*(undefined1**)this = &m_aNull;
	}
	**this = 0;
	return (undefined4*)this;
		*/
	}

	TCString::TCString(const TCString& src)
	{
		Reset();
		Copy(src, -1);
	}

	TCString::TCString(uint32_t size)
	{
		Reset();
		AllocBuffer(size, true);
	}

	TCString::TCString(const TWString& src)
	{
		Reset();
		Copy(src, -1);
	}

	TCString::TCString(const char* const& src)
	{
		Reset();
		Copy(src, -1);
	}

	TCString::~TCString()
	{
		FreeBuffer();
	}

	void TCString::Reset()
	{
		m_pBuffer = NullString;
		m_iStrLen = 0;
		m_iExcessLen = 0;
	}

	void TCString::Copy(const TWString& src, uint32_t size)
	{
		uint32_t srcLen = src.Length();
		TASSERT(srcLen <= 0xFFFFFF, "Too big string");

		if (srcLen < size || size == -1) { size = srcLen; }

		AllocBuffer(size, true);
		TSystem::StringUnicodeToChar(m_pBuffer, src, size);
		m_pBuffer[size] = 0;
	}

	void TCString::Copy(const char* src, uint32_t size)
	{
		if (src != m_pBuffer)
		{
			size_t srcLen = src ? TSystem::StringLength(src) : 0;
			TASSERT(srcLen <= 0xFFFFFF, "Too big string");

			if (srcLen < size || size == -1)
			{
				size = (uint32_t)srcLen;
			}

			AllocBuffer(size, true);
			TSystem::MemCopy(m_pBuffer, src, size);

			m_pBuffer[size] = 0;
		}
	}

	uint32_t TCString::Find(char substr, int pos) const
	{
		if (!IsIndexValid(0)) return -1;
		//if (DAT_00990290 == 0)

		char* foundAt = strchr(&m_pBuffer[pos], substr);
		if (foundAt == TNULL) { return -1; }

		return (uint32_t)(foundAt - m_pBuffer);
	}

	bool TCString::AllocBuffer(int a_iLength, bool freeMemory)
	{
		bool hasChanged = false;
		uint32_t currentLength = Length();

		TASSERT(a_iLength >= 0, "Length can't be less than 0");

		if (a_iLength != currentLength)
		{
			if (a_iLength == 0)
			{
				if (freeMemory) tfree(m_pBuffer);

				m_pBuffer = NullString;
				m_iExcessLen = 0;

				hasChanged = true;
			}
			else
			{
				int newExcessLen = (currentLength - a_iLength) + m_iExcessLen;

				if (newExcessLen < 0 || newExcessLen > 0xFF)
				{
					if (currentLength != 0 && freeMemory)
					{
						tfree(m_pBuffer);
					}

					m_pBuffer = (char*)tmalloc(a_iLength + 1);
					m_iExcessLen = 0;

					hasChanged = true;
				}
				else
				{
					m_iExcessLen = newExcessLen;
					hasChanged = false;
				}
			}

			m_iStrLen = a_iLength;
		}

		if (freeMemory) m_pBuffer[0] = '\0';
		return hasChanged;
	}

	void TCString::FreeBuffer()
	{
		if (Length() != 0)
		{
			tfree(m_pBuffer);
		}

		Reset();
	}

	const char* TCString::GetString(uint32_t a_iIndex) const
	{
		TASSERT(a_iIndex >= 0 && a_iIndex <= (TINT)m_iStrLen, "");
		if (IsIndexValid(a_iIndex)) { return &m_pBuffer[a_iIndex]; }
		return TNULL;
	}

	TCString& TCString::Concat(const char* str, uint32_t size)
	{
		uint32_t len = (uint32_t)TSystem::StringLength(str);
		
		if ((len < size) || (size == -1))
		{
			size = len;
		}

		bool allocated = AllocBuffer(m_iStrLen + size, false);
		if (allocated)
		{
			//?????
			TSystem::StringCopy(m_pBuffer, m_pBuffer, -1);
		}
		TSystem::StringCopy(m_pBuffer + m_iStrLen, str, size);
		m_pBuffer[m_iStrLen + size] = 0;
		if (allocated && m_iStrLen != 0)
		{
			FreeBuffer();
		}
		return *this;
	}

	TCString& TCString::Concat(const TWString& str, uint32_t size)
	{
		uint32_t len = str.Length();

		if ((len < size) || (size == -1))
		{
			size = len;
		}

		bool allocated = AllocBuffer(m_iStrLen + size, false);
		if (allocated)
		{
			//?????
			TSystem::StringCopy(m_pBuffer, m_pBuffer, -1);
		}
		TSystem::StringUnicodeToChar(m_pBuffer + m_iStrLen, str.GetString(), size);
		m_pBuffer[m_iStrLen + size] = 0;
		if (allocated && m_iStrLen != 0)
		{
			FreeBuffer();
		}
		return *this;
	}

	/* Operators */

	TCString* TCString::operator+=(char const* str)
	{
		Concat(str, -1);
		return this;
	}

	char& TCString::operator[](int index)
	{
		return m_pBuffer[index];
	}
}