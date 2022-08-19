#pragma once
#include "TFile.h"
#include "Toshi/Typedefs.h"

namespace Toshi
{
	class TNativeFile
	{
		HANDLE hnd; // 0x8
		int m_position; // 0xC
		DWORD unk2; //0x10
		int unk4; // 0x14
		int unk5; // 0x18
		LPCVOID m_pBuffer; // 0x1C
		LPCVOID buffer; // 0x20
		int m_iWriteBufferUsed; // 0x24
		bool m_bWriteBuffered; // 0x28

	public:
	//private:
		bool LoadBuffer(LPVOID, DWORD);
		int FlushWriteBuffer();
		virtual int Read(LPVOID, int);
		virtual int Tell();
		virtual bool Seek(int, TFile::TSEEK);
	//protected:
		bool Open(const TCString*, unsigned int);
		void Close();
	};
}


