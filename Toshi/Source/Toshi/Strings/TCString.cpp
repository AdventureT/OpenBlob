#include "pch.h"
#include "TCString.h"

#include "Toshi/Typedefs.h"
#include "Toshi/Core/TMemory.h"
#include "Toshi/Core/TSystem.h"

namespace Toshi
{
    TCString::TCString()
    {
        m_pBuffer = nullptr;
        length = length & 0xFF000000;
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

	TCString::TCString(uint32_t length)
	{
		this->m_pBuffer = TNULL;
		length = length & 0xff000000;
		//this + 7 = 0
		//TODO
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

	void TCString::Copy(const TWString& src, uint32_t size)
	{
		uint32_t srcLen = src.Length();
		if (srcLen < size || size == -1) { size = srcLen; }

		AllocBuffer(size, true);
		TSystem::StringUnicodeToChar(m_pBuffer, src, size);
		m_pBuffer[size] = 0;
	}

    void TCString::Copy(const char* src, uint32_t a_iLength)
    {
        if (src != m_pBuffer)
        {
            uint32_t strLength = src ? TSystem::StringLength(src) : 0;

            if (strLength < a_iLength || a_iLength == -1)
            {
                a_iLength = strLength;
            }

            AllocBuffer(a_iLength, true);
            TSystem::MemCopy(m_pBuffer, src, a_iLength);

            m_pBuffer[a_iLength] = 0;
        }
    }

    int TCString::Find(char substr, int pos) const
    {
        if (!IsIndexValid(0)) return -1;
        //if (DAT_00990290 == 0)

        char* foundAt = strchr(&m_pBuffer[pos], substr);
        if (foundAt == TNULL) { return -1; }
        
        return foundAt - m_pBuffer;
    }

    bool TCString::AllocBuffer(int a_iLength, bool freeMemory)
    {
        bool rVal = false;
        uint32_t currentLength = Length();

        TASSERT(a_iLength >= 0, "Length can't be less than 0");

        if (a_iLength == currentLength)
        {
            rVal = false;
        }
        else
        {
            if (a_iLength == 0)
            {
                if (freeMemory) { tfree(m_pBuffer); }
                
                m_pBuffer = nullptr;
                rVal = true;
                length = (length & ~(0xFF << 24)) | (0 << 24);
                //this+7 = 0
            }
            else
            {
                int iVar2 = (currentLength - a_iLength) + (length & 0x000000FF); // + this[7]
                if ((iVar2 < 0) || (0xFF < iVar2))
                {
                    if ((currentLength != 0) && freeMemory)
                    {
                        tfree(m_pBuffer);
                    }

                    m_pBuffer = (char*)tmalloc(a_iLength + 1);
                    length = (length & ~(0xFF << 24)) | (0 << 24);
                    //this + 7 = 0
                }
                else
                {
                    length = (length & ~(0xFF << 24)) | (iVar2 << 24);
                    //this + 7 = (char) uVar1;
                    rVal = false;
                }
            }

            length = length & 0xFF000000 | a_iLength & 0xFFFFFF;
        }
        /*
        if (freeMemory)
        {
            m_pBuffer = nullptr;
        }*/
        return rVal;
    }

    void TCString::FreeBuffer()
    {
        if (Length() != 0)
        {
            tfree(m_pBuffer);
            m_pBuffer = nullptr;
        }

        Reset();
    }

    const char* TCString::GetString(uint32_t index) const
    {
        if (IsIndexValid(index)) { return &m_pBuffer[index]; }
        return TNULL;
    }

    void TCString::Reset()
    {
        m_pBuffer = TNULL;
        length &= 0xFF000000;
        length = (length & ~(0xFF << 24)) | (0 << 24);

        // this + 7 = 0;
    }
    
    /* Operators */

	TCString* TCString::operator+=(char const* str)
	{
        TIMPLEMENT();
		return nullptr;
	}
}