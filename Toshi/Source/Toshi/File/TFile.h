#pragma once
#include "Toshi/Utils/TSingleton.h"
#include "Toshi/Strings/TCString.h"

#ifdef CreateFile
#undef CreateFile
#endif

namespace Toshi
{
	class TFile;

	// JPOG Matched
	class TFileSystem
	{
	public:
		TFileSystem(TFileSystem const&);
		TFileSystem(const char*);
		virtual ~TFileSystem();

		// Override these funcs in TNativeFileSystem
		virtual TFile* CreateFile(TCString const& fn, uint32_t flags) = 0;
		virtual void DestroyFile(TFile*) = 0;
		virtual bool RemoveFile(TCString const&) { return true; }
		virtual TCString MakeInternalPath(TCString const&) = 0;
		virtual bool GetFirstFile(TCString const&, TCString&, unsigned int) { return false; }
		virtual bool GetNextFile(TCString&, uint32_t) { return false; }
		virtual void SetPrefix(TCString const&);
		virtual bool MakeDirectory(TCString const&) = 0;

		inline TCString const& GetName() const { return m_sName; }
		inline TCString const& GetPrefix() const { return m_prefix; }

		TFileSystem& operator=(TFileSystem& a_rFileSystem);

	public:
		// Creates native file system based on the used platform
		static STL::Ref<TFileSystem> CreateNative(const char* name);
	
	protected:
		void* m_unk; // 0x4
		void* m_unk2; // 0x8

		TCString m_sName; // 0xC
		TCString m_prefix; // 0x14
		int m_unk3; // 0x1C
	};

	class TFileManager : public TSingleton<TFileManager>
	{
	public:
		TCString m_sSysPath; // 0x4
		TCString m_sWorkingDirectory; // 0xC
		TMutex m_mMutex; // 0x24


		TFileManager() = default;

		// JPOG Matched
		class TSysPathIter
		{
			TCString m_sSysPath;
			int m_position;
		public:
			bool First(TCString & param_1);
			bool Next(TCString& param_1);

			TSysPathIter(const TCString&);
			TSysPathIter(const TSysPathIter&);
		};

	private:
		void ValidateSystemPath();

	public:
		void MountFileSystem(TFileSystem* a_pFileSystem);
		class TFileSystem* FindFileSystem(const TCString&);
		class TFile* CreateFile(const TCString&, unsigned int);
	};

	class TFile
	{
	public:
		enum TSEEK
		{
			TSEEK_SET,
			TSEEK_CUR,
			TSEEK_END
		};

		enum OpenFlags
		{
			OpenFlags_Read = BITFIELD(0),
			OpenFlags_Write = BITFIELD(1),
			OpenFlags_ReadWrite = BITFIELD(2),
			OpenFlags_CreateNew = BITFIELD(3),
			OpenFlags_NoBuffer = BITFIELD(4),
		};

		//TFile();
		TFile(TFileSystem*);
		TFile(const TFile&);

		virtual int Read(LPVOID, int) = 0;
		virtual int Write(LPVOID, int) = 0;
		virtual bool Seek(int, TFile::TSEEK) = 0;
		virtual int Tell() = 0;
		virtual int GetSize()  = 0;
		virtual _FILETIME GetDate() { return {}; }
		virtual char GetCChar() = 0;
		virtual wchar_t GetWChar() = 0;
		virtual int PutCChar(char character) = 0;
		virtual int PutWChar(wchar_t character) = 0;
		virtual int VCPrintf(const char* format, ...) = 0;
		virtual int VWPrintf(const wchar_t* format, ...) = 0;

		static TFile* Create(const TCString& fn, uint32_t flags = OpenFlags_Read);
		inline TFileSystem* GetFileSystem() const { return m_pFileSystem; }
		inline TFile& operator=(const TFile& a_pFile) { m_pFileSystem = a_pFile.GetFileSystem(); return *this; }

	public:
		TFileSystem* m_pFileSystem;
	};
}


