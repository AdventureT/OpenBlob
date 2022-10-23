#include "ToshiPCH.h"
#include "TFile.h"

namespace Toshi
{
#pragma region TFile

	TFile::TFile(TFileSystem* pFS)
	{
		m_pFileSystem = pFS;
	}

	TFile::TFile(const TFile& other)
	{
		m_pFileSystem = other.GetFileSystem();
	}

	TFile* TFile::Create(const TString8& filename, uint32_t flags)
	{
		TASSERT(filename.IsIndexValid(0), "filename is empty");

		TFile* pFile = TFileManager::GetSingleton()->CreateFile(filename, flags);

		if (pFile != TNULL)
		{
			TOSHI_CORE_TRACE("TFile::Create - Created file {0} with mode {1}", filename, flags);
		}
		else
		{
			TOSHI_CORE_TRACE("TFile::Create - Failed to create file {0} with mode {1}", filename, flags);
		}

		return pFile;
	}

	TString8 TFile::ConcatPath(const TString8& a_rcString, const TString8& a_rcString2)
	{
		TIMPLEMENT();
		return { };
	}

#pragma endregion

#pragma region TFileManager

	TFileManager::TFileManager() : m_WorkingDirectory("/"), m_ValidatedCount(0), m_Mutex()
	{
		InvalidateSystemPath();
	}

	TFileManager::~TFileManager()
	{
		Destroy();
	}

	void TFileManager::Destroy()
	{
		{
			auto pFileSystem = TFileManager::FindFileSystem("mem");
			if (pFileSystem) delete pFileSystem;
		}

		{
			auto pFileSystem = TFileManager::FindFileSystem("null");
			if (pFileSystem) delete pFileSystem;
		}

		{
			auto pFileSystem = TFileManager::FindFileSystem("abs");
			if (pFileSystem) delete pFileSystem;
		}

		{
			auto pFileSystem = TFileManager::FindFileSystem("local");
			if (pFileSystem) delete pFileSystem;
		}
	}

	void TFileManager::MountFileSystem(TFileSystem* pFS)
	{
		// FUN_00685860
		TASSERT(TFileManager::FindFileSystem(pFS->GetName()) == TNULL, "This TFileSystem ({0}) is already mounted", pFS->GetName());
		TASSERT(!pFS->IsLinked(), "TFileSystem shouldn't be linked");
		
		m_Invalidated.InsertTail(pFS);
		InvalidateSystemPath();
	}

	TFileSystem* TFileManager::FindFileSystem(const TString8& name)
	{
		// FUN_00685cc0
		TFileSystem* pFileSystem = TFileManager::FindFileSystem(m_Validated, name);

		if (pFileSystem == TNULL)
		{
			pFileSystem = TFileManager::FindFileSystem(m_Invalidated, name);
		}

		return pFileSystem;
	}

	TFileSystem* TFileManager::FindFileSystem(TDList<TFileSystem>& list, const TString8& name)
	{
		auto pNode = list.Begin();

		while (pNode != list.End())
		{
			if (pNode->GetName() == name) return pNode;
			pNode = pNode->NextNode();
		}

		return TNULL;
	}

	TFile* TFileManager::CreateFile(const TString8& a_sName, uint32_t flags)
	{
		// FUN_006854c0
		TASSERT(a_sName.Length() > 0, "Name can't be empty");

		ValidateSystemPath();
		int pos = a_sName.Find(':', 0);

		if (pos >= 0)
		{
			TString8 str1;
			TString8 str2;

			str1.Copy(a_sName, pos);
			str2.Copy(a_sName.GetString(pos + 1), -1);
			TFileSystem* fs = FindFileSystem(str1);

			if (fs != TNULL)
			{
				return fs->CreateFile(str2, flags);
			}
		}

		auto pNode = m_Validated.Begin();

		while (pNode != m_Validated.End())
		{
			TFile* pFile = pNode->CreateFile(a_sName, flags);
			if (pFile != TNULL) return pFile;

			pNode = pNode->NextNode();
		}

		return nullptr;
	}

	void TFileManager::ValidateSystemPath()
	{
		if (!m_IsValidated)
		{
			auto pNode = m_Validated.Tail();

			while (pNode != m_Validated.End())
			{
				pNode->RemoveNode();
				m_Invalidated.InsertHead(pNode);

				pNode = m_Validated.Tail();
			}

			m_ValidatedCount = 0;

			TString8 fsName;
			TSysPathIter pathIter(m_SysPath);

			bool hasPath = pathIter.First(fsName);

			while (hasPath)
			{
				TFileSystem* pFS = FindFileSystem(m_Invalidated, fsName);

				if (pFS != TNULL)
				{
					pFS->RemoveNode();
					m_Validated.InsertTail(pFS);
					m_ValidatedCount += 1;
				}

				hasPath = pathIter.Next(fsName);
			}

			m_IsValidated = true;
		}
	}

#pragma endregion
	
#pragma region TFileSystem

	TFileSystem::TFileSystem(const TFileSystem& other)
	{
		m_Name = TString8(other.GetName());
		m_Prefix = TString8(other.GetPrefix());
	}

	TFileSystem::TFileSystem(const char* name) : m_Name(name), m_Prefix()
	{
		//???
		// m_unk = m_unk
		m_Handle = NULL;
	}

	void TFileSystem::SetPrefix(const TString8& prefix)
	{
		m_Prefix = prefix;

		for (uint32_t i = 0; i < m_Prefix.Length(); i++)
		{
			if (m_Prefix[i] == '/')
			{
				m_Prefix[i] = '\\';
			}
		}
	}

	TFileSystem& TFileSystem::operator=(TFileSystem& a_rFileSystem)
	{
		m_Name = a_rFileSystem.GetName();
		m_Prefix = a_rFileSystem.GetPrefix();
		return *this;
	}

#pragma endregion
	TFile* TNullFileSystem::CreateFile(TString8 const& fn, uint32_t flags)
	{
		TIMPLEMENT();
		return nullptr;
	}

	void TNullFileSystem::DestroyFile(TFile*)
	{
		TIMPLEMENT();
	}

	TString8 TNullFileSystem::MakeInternalPath(TString8 const&)
	{
		TIMPLEMENT();
		return TString8();
	}

	bool TNullFileSystem::MakeDirectory(TString8 const&)
	{
		TIMPLEMENT();
		return false;
	}
}
