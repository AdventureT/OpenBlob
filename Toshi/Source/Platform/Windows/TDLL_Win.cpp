#include "ToshiPCH.h"
#include "TDLL_Win.h"

namespace Toshi {

	TDLL::TDLL()
	{
		m_pDLL = TNULL;
	}

	TDLL::TDLL(const TDLL& other)
	{
		m_sFileName = other.m_sFileName;
		m_pDLL = other.m_pDLL;
	}

	TDLL::~TDLL()
	{
		Unload();
	}

	TBOOL TDLL::Load(const TCString& a_sFileName)
	{
		Unload();
		m_sFileName = a_sFileName;
		m_pDLL = LoadLibraryA(m_sFileName);
		TASSERT(m_pDLL != TNULL);

		return m_pDLL != TNULL;
	}

	void TDLL::Unload()
	{
		if (m_pDLL != NULL)
		{
			FreeLibrary(m_pDLL);
			m_sFileName = "";
			m_pDLL = NULL;
		}
	}

	void* TDLL::GetAddress(const TCString& a_sSymbolName)
	{
		void* pAddress = GetProcAddress(GetDLL(), a_sSymbolName);
		TASSERT(pAddress != TNULL);

		return pAddress;
	}

}