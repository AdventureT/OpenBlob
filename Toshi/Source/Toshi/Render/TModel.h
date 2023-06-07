#pragma once
#include "TTMDBase.h"
#include "TSkeleton.h"
#include "TModelCollision.h"

#include "Toshi/File/TTRB.h"
#include "Toshi2/T2ResourceManager.h"

namespace Toshi {

	class TModel : public T2Resource
	{
	public:
		enum class Flags
		{
			None      = 0,
			Created   = BITFIELD(0),
			TrbLoaded = BITFIELD(1),
		};

		using t_TRBLoadCallback = TBOOL(*)(TTRB& pTRB, TModel* pModel);

	public:
		TModel();
		~TModel();

		void Create(const char* name, TBOOL bLoadImmediately);
		void Delete();
		void Unload();
		
		void SetTRB(TTRB* pTRB, TBOOL bFreeOnUnload)
		{
			m_pTRB = pTRB;
			m_bFreeOnUnload = bFreeOnUnload;
		}
		
		TBOOL LoadTRB();
		TBOOL LoadTRB(TTRB* pTRB, TBOOL bFreeOnUnload);

		TBOOL LoadTRBFile(const char* filepath);
		TBOOL LoadTRBFile(TFile* pFile);

		void UnloadTRB(TBOOL bFreeTrb);

		TBOOL IsCreated() const
		{
			return m_Flags.IsSet(Flags::Created);
		}

	protected:
		void CreateResource(const char* name);

		void CreateSkeleton(TTMDBase::SkeletonHeader* pSkeletonHeader, TSkeleton* pSkeleton, TBOOL bLoadAnimations);
		void CreateCollision(TModelCollision* pModelCollision);

	public:
		static void* ResourceCallback(void* pData, TTRB* pTRB, TBOOL flag);
		
		static const char* TranslateSymbolName(const char* symbolName);

		static void SetTRBLoadCallback(t_TRBLoadCallback fnCallback)
		{
			sm_pTRBLoadCallback = fnCallback;
		}

	protected:
		inline static const char* sm_SymbolNamePrefix = TNULL;
		inline static t_TRBLoadCallback sm_pTRBLoadCallback;

	protected:
		T2Flags<Flags> m_Flags;        // 0x04
		int m_iLODCount;               // 0x0C
		TSkeleton* m_pSkeleton;        // 0x14
		TModelCollision* m_pCollision; // 0xA0
		TTRB* m_pTRB;                  // 0xA4
		TBOOL m_bFreeOnUnload;         // 0xA8
		const char* m_pResourceName;   // 0xAC
		T2ResourcePtr m_ResourcePtr1;  // 0xB0
		T2ResourcePtr m_ResourcePtr2;  // 0xB4
	};

	DEFINE_T2FLAGS(TModel::Flags);

}

/*
#include "Toshi/File/TFile.h"
#include "Toshi/Render/TSkeleton.h"
#include "Toshi2/T2ResourceManager.h"
*/
/*
namespace Toshi
{

	class TModelLOD
	{
		int m_iMeshCount; // 0x10
	public:
		inline int GetMeshCount() { return m_iMeshCount; }
	};

	class TModel
	{
		unsigned int m_Flags; // 0x4
		//int m_instanceCount; // 0x8
		int m_iLODCount; // 0xC
		TSkeleton* m_pSkeleton; // 0x14
		//int* m_pUVHashOffset; // 0x18
		TModelLOD* m_modelLODs; // 0x18
		TTRB* m_pTRB; // 0xA4
		const char* m_modelName; // 0xDC NT08, 0xAC Deblob

		static char* sm_SymbolNamePrefix;
		static char s_TranslatedSymbol[0x200];

		struct TTMDBase
		{
			struct SkeletonHeader
			{

			};
		};

		struct Header
		{
			uint32_t m_magic;
			uint32_t m_version;
			uint32_t m_remainingBytes;
		};

		struct Header2
		{

		};

		bool Create(const char* name, bool a_bLoadImmediately);
		void CreateResource(const char* name);

		inline bool IsCreated() { return (m_Flags & 1) != 0; }

		inline TModelLOD& GetLOD(int index) { return m_modelLODs[index]; }
		inline int GetLODCount() { return m_iLODCount; }



	protected:
		bool LoadTMD(const char*); // JPOG only i think not sure though
		bool LoadTRBTMD(const char*);
		bool LoadTrb();
		const char* TranslateSymbolName(const char* a_symbolName);
	};
	
}

*/