#pragma once
#include "Toshi/File/TTRB.h"
#include "Toshi/Thread/TThread.h"
#include "Toshi/Utils/TUtil.h"

#include <cstdint>

namespace Toshi
{
	class T2ResourceData;

	class T2ResourceManager :
		public TSingleton<T2ResourceManager>
	{
	public:
		using t_CreateDestroyCallbk = void* (*)(void* pCustomData, TTRB* trb, TBOOL bCreated);

	public:
		T2ResourceManager(int a_iMaxNumResources);
		~T2ResourceManager();

		int CreateResource(const char* resourceName, void* pData, t_CreateDestroyCallbk a_fnCreateDestroyCallbk, void* pCallbkData);
		void DestroyResource(int a_iID);
		
		void IncRefCount(int a_iID);
		void DecRefCount(int a_iID);
		
		int FindUnusedResource();

		T2ResourceData* GetResourceData(int a_iID);
		void* GetData(int a_iID);

	private:
		int m_iMaxNumResources;
		int m_iNumUsedResources;
		int m_iUnk;
		T2ResourceData* m_pData;
	};

	class T2ResourceData
	{
	public:
		static constexpr int MAX_RESOURCE_NAME = 48;

		friend class T2ResourceManager;

	public:
		enum FLAG : uint8_t
		{
			FLAG_INITIALISED = BITFIELD(0),
			FLAG_LOADING = BITFIELD(1),
			FLAG_DESTROYED = BITFIELD(2),
			FLAG_USED = BITFIELD(3),
		};

		void Init(const char* a_pName, T2ResourceManager::t_CreateDestroyCallbk a_fnCreateDestroyCallbk, void* a_pCallbkData);
		void DeInit();

		void Load(const char* filepath);
		void Unload();

		void SetLoadedData(void* a_pData);
		void* GetData();

		bool HasFlag(uint8_t flag) const
		{
			return (m_iFlags & flag) == flag;
		}

	private:
		void* m_pData;
		TTRB* m_pTRB1;
		int m_iFlags;
		int m_iRefCount;
		char* m_pResourceName;
		void* m_pCreateDestroyCallbkData;
		T2ResourceManager::t_CreateDestroyCallbk m_fnCreateDestroyCallbk;
		TTRB* m_pTRB2;
	};

	class T2ResourcePtr
	{
	public:
		static constexpr int IDINVALID = 0;

	public:
		T2ResourcePtr()
		{
			m_iResourceID = IDINVALID;
		}

		explicit T2ResourcePtr(int a_iID)
		{
			m_iResourceID = a_iID;
			T2ResourceManager::GetSingletonWeak()->IncRefCount(m_iResourceID);
		}

		T2ResourcePtr(const T2ResourcePtr& a_resourcePtr)
		{
			m_iResourceID = a_resourcePtr.m_iResourceID;
			T2ResourceManager::GetSingletonWeak()->IncRefCount(m_iResourceID);
		}

		~T2ResourcePtr()
		{
			T2ResourceManager::GetSingletonWeak()->DecRefCount(m_iResourceID);
		}

		operator bool() const
		{
			return m_iResourceID != IDINVALID;
		}

	private:
		int m_iResourceID;
	};

	class T2Resource
	{
	public:
		void CreateResource(const char* resourceName, void* pData, T2ResourceManager::t_CreateDestroyCallbk a_fnCreateDestroyCallbk, void* pCallbkData);
		void DestroyResource();

		T2ResourcePtr GetResourcePtr() const
		{
			return T2ResourcePtr(m_iID);
		}

	private:
		int m_iID = 0;
	};
}