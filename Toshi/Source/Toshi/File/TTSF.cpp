#include "ToshiPCH.h"
#include "TTSF.h"
#include "TTRB.h"

namespace Toshi
{
	uint8_t TTSF::ReadFile(TFile* a_pFile)
	{
		// FUN_00686920

		m_pFile = a_pFile;
		m_FileInfoCount = 0;

		if (m_pFile == TNULL)
		{
			return TTRB::ERROR_NO_FILE;
		}

		m_pFile->Read(&m_Header, sizeof(TTSF::Header));

		if (m_Header.m_Magic == TMAKEFOUR("TSFL"))
		{
			m_iEndianess = Endianess::Little;
		}
		else
		{
			if (m_Header.m_Magic != TMAKEFOUR("TSFB"))
			{
				return TTRB::ERROR_NOT_TRB;
			}

			m_iEndianess = Endianess::Big;
		}

		m_ReadPos += m_pFile->Read(&m_TRBF, 4);

		if (m_iEndianess == Endianess::Big)
		{
			m_Header.m_FileSize = BIG_ENDIAN_TO_LITTLE_UINT32(m_Header.m_FileSize);
			m_TRBF = BIG_ENDIAN_TO_LITTLE_UINT32(m_TRBF);
		}

		m_FileMagic = m_Header.m_Magic;
		m_FileSize = m_Header.m_FileSize;
		SaveFileInfo();

		return TTRB::ERROR_OK;
	}

	uint8_t TTSF::SaveFileInfo()
	{
		// FUN_00686130

		if (m_FileMagic != TMAKEFOUR("FORM") && m_FileMagic != TMAKEFOUR("TSFL") && m_FileMagic != TMAKEFOUR("TSFB"))
		{
			return TTRB::ERROR_WRONG_MAGIC;
		}

		m_FileInfo[m_FileInfoCount].FileStartOffset = m_pFile->Tell() - 4;
		m_FileInfo[m_FileInfoCount].FileSize = m_FileSize;
		m_FileInfoCount++;

		return TTRB::ERROR_OK;
	}

	uint8_t TTSF::FUN_006881B0()
	{
		if (m_FileInfoCount < 1) return TTRB::ERROR_WRONG_MAGIC;

		m_FileInfoCount--;
		m_FileSize = m_Unk[m_FileInfoCount];

		m_ReadPos = m_pFile->Tell() - m_FileInfo[m_FileInfoCount].FileStartOffset;

		int thing = m_FileSize + 3U & 0xfffffffc;

		m_pFile->Seek(thing - m_ReadPos, TFile::TSEEK_CUR);
		m_ReadPos += thing;

		return TTRB::ERROR_OK;
	}

	uint8_t TTSF::FUN_00687FA0()
	{

		return uint8_t();
	}

	TTSF::TTSF()
	{
		m_pFile = TNULL;
		m_FileInfoCount = 0;
		m_ReadPos = 0;
		// FUN_007EC9F0(m_FileInfo, 0, 0x100);
		m_TRBF = 0;
		m_FileMagic = 0;
	}
}