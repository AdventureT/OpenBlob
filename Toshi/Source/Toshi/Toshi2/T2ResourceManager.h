#pragma once
#include <cstdint>
#include "Toshi/File/TTRB.h"
#include "Toshi/Core/TThread.h"

namespace Toshi
{
	using t_CreateDestroyCallbk = void* (*)(void* unk, TTRB* trb, bool unk1);

	class T2ResourceData;

	/*
	class T2Resource
	{
		
	};
	*/

	class T2ResourcePtr
	{
	public:
		static const int IDINVALID = 0;
	};

	class T2ResourceManager
	{
		
		T2ResourceData& GetResourceData(int a_iID);
		void CreateResource(const char* resourceName, void* unk) { TTODO(""); };

		int m_iMaxNumResources;		// 0x0
		T2ResourceData* m_pData;	// 0xC
	};

	// sizeof(T2ResourceData) == 0x4C!
	class T2ResourceData
	{
	public: 
		enum FLAG
		{
			FLAG_LOADING = 1,
			FLAG_INITIALISED = 2,
			FLAG_DESTROYED = 3,
			FLAG_UNK = 4
		};

		void Init(const char* a_pName, t_CreateDestroyCallbk a_fnCreateDestroyCallbk, void* a_pData);
		void SetLoadedData(void* a_pData);
		void Unload();
		void* GetData();
		inline bool HasFlag(FLAG flag) { return (m_iFlags & flag) != 0; }
	private:
		void* m_pData;										// 0x0
		int m_iFlags;										// 0x8
		int m_iRefCount;									// 0xC
		char* m_pResourceName;								// 0x10
		void* m_unk;										// 0x40
		t_CreateDestroyCallbk m_fnCreateDestroyCallbk;		// 0x44
		TTRB* m_trb;										// 0x48
	};
}