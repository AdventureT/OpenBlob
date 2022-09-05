#pragma once
#include "Toshi/Typedefs.h"
#include "TFile.h"
#include "TNativeFileSystem.h"

namespace Toshi
{
	class TNativeFile : public TFile
	{
		HANDLE hnd; // 0x8
		int m_position; // 0xC
		DWORD unk2; //0x10
		int unk4; // 0x14
		int unk5; // 0x18
		LPVOID m_pBuffer; // 0x1C
		LPVOID buffer; // 0x20
		int m_iWriteBufferUsed; // 0x24
		bool m_bWriteBuffered; // 0x28

	public:
	//private:
		bool LoadBuffer(DWORD);
		int FlushWriteBuffer();
		int ReadUnbuffered(LPVOID, int);
		virtual int Read(LPVOID, int);
		virtual int Tell();
		virtual bool Seek(int, TFile::TSEEK);
		virtual int GetCChar();
		virtual int GetSize();
		virtual _FILETIME GetDate();
	//protected:
		TNativeFile(class TNativeFileSystem *);
		TNativeFile(const TNativeFile&);
		bool Open(const TCString& a_FileName, uint32_t a_Flags);
		void Close();
	};
}


