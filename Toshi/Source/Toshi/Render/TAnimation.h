#pragma once
#include "Toshi/Core/TQList.h"
#include "Toshi/File/TTRB.h"

#ifdef LoadLibrary
#undef LoadLibrary
#endif LoadLibrary

namespace Toshi {
	
	extern TMemoryHeap* s_pAnimMemHeap;
	constexpr int TANIMATION_MAXBONES = 128;

	using TAnimVector = TVector3;
	using TAnimQuaternion = TQuaternion;
	using TAnimScale = TFLOAT;
	using TAnimationBone = unsigned short;

	class TSkeletonInstance;

	class TAnimation : public TQList<TAnimation>::TNode
	{
	public:
		using Flags = uint16_t;
		enum Flags_ : Flags
		{
			Flags_None = 0,
			Flags_Active = BITFIELD(0),
			Flags_Managed = BITFIELD(1),
			Flags_Unknown2 = BITFIELD(2),
			Flags_UpdateStateOnRemove = BITFIELD(3),
		};

		using Mode = uint32_t;
		enum Mode_ : Mode
		{
			MODE_UNK1,
			MODE_UNK2,
			MODE_UNK3,
		};

	public:
		TAnimation() = default;

		TBOOL UpdateTime(float a_fDeltaTime);
		void RemoveAnimation(float a_fBlendOutSpeed);

		void SetMode(Mode a_eMode) { m_eMode = a_eMode; }
		void SetSkeletonInstance(TSkeletonInstance* a_pSkeletonInstance) { m_pSkeletonInstance = a_pSkeletonInstance; }
		float SetSpeed(float a_fSpeed) { return std::exchange(m_fSpeed, a_fSpeed); }
		float SetDestWeight(float a_fDestWeight, float a_fBlendInSpeed);
		void SetDestWeightExplicit(float a_fDestWeight) { m_fDestWeight = a_fDestWeight; }
		void SetBlendInSpeed(float a_fBlendInSpeed) { m_fBlendInSpeed = a_fBlendInSpeed; }
		void SetBlendOutSpeed(float a_fBlendOutSpeed) { m_fBlendOutSpeed = a_fBlendOutSpeed; }
		void SetWeight(float a_fWeight) { m_fWeight = a_fWeight; }
		void SetFlags(Flags a_eFlags) { m_eFlags = a_eFlags; }
		void SetUnk3(short a_iUnk3) { m_iUnk3 = a_iUnk3; }
		void SetSequence(unsigned short a_iSeqID) { m_iSeqID = a_iSeqID; }

		void SetUpdateStateOnRemove(TBOOL a_bEnable)
		{
			if (a_bEnable)
				m_eFlags |= Flags_UpdateStateOnRemove;
			else
				m_eFlags &= ~Flags_UpdateStateOnRemove;
		}

		void ResetTime()
		{
			m_fTotalTime = 0.0f;
			m_fSeqTime = 0.0f;
		}

		Flags GetFlags() const { return m_eFlags; }
		Mode GetMode() const { return m_eMode; }
		TBOOL IsActive() const { return m_eFlags & Flags_Active; }
		TBOOL IsManaged() const { return m_eFlags & Flags_Managed; }
		TBOOL IsUpdateStateOnRemove() const { return m_eFlags & Flags_UpdateStateOnRemove; }

		unsigned short GetSequence() const { return m_iSeqID; }
		class TSkeletonSequence* GetSequencePtr() const;
		TSkeletonInstance* GetSkeletonInstance() const { return m_pSkeletonInstance; }
		float GetSpeed() const { return m_fSpeed; }
		float GetSeqTime() const { return m_fSeqTime; }
		float GetTotalTime() const { return m_fTotalTime; }
		float GetWeight() const { return m_fWeight; }
		float GetDestWeight() const { return m_fDestWeight; }
		float GetBlendInSpeed() const { return m_fBlendInSpeed; }
		float GetBlendOutSpeed() const { return m_fBlendOutSpeed; }

		float& GetWeight() { return m_fWeight; }

		TAnimationBone* GetBones() { return TREINTERPRETCAST(TAnimationBone*, this + 1); }
		TAnimationBone* GetBone(int a_iIndex) { return &TREINTERPRETCAST(TAnimationBone*, this + 1)[a_iIndex]; }

	public:
		static int FindSequenceMaxUnk3(const TQList<TAnimation>& a_rList);

	private:
		TSkeletonInstance* m_pSkeletonInstance;
		unsigned short m_iSeqID;
		Flags m_eFlags;
		short m_iUnk3;
		Mode m_eMode;
		float m_fSpeed;
		float m_fWeight;
		float m_fDestWeight;
		float m_fTotalTime;
		float m_fSeqTime;
		float m_fBlendInSpeed;
		float m_fBlendOutSpeed;
	};

	class TKeyframeLibrary : public TDList<TKeyframeLibrary>::TNode
	{
	public:
		friend class TKeyframeLibraryInstance;

		struct TRBHeader
		{
			const char* m_szName;
			TVector3 m_SomeVector;
			int m_iNumTranslations;
			int m_iNumQuaternions;
			int m_iNumScales;
			int m_iTranslationSize;
			int m_iQuaternionSize;
			int m_iScaleSize;
			TAnimVector* m_pTranslations;
			TAnimQuaternion* m_pQuaternions;
			TAnimScale* m_pScales;
		};

	public:
		static TKeyframeLibrary* Create(TRBHeader* a_pTRBHeader);

		void Delete();

		const char* GetName() const
		{
			return m_szName;
		}

	private:
		TKeyframeLibrary();

		void IncreaseRefCount()
		{
			m_iReferenceCount++;
		}

		void DecreaseRefCount()
		{
			m_iReferenceCount--;
		}

	private:
		const char* m_szName;
		TVector3 m_SomeVector;
		int m_iReferenceCount;
		int m_iNumTranslations;
		int m_iNumQuaternions;
		int m_iNumScales;
		int m_iTranslationSize;
		int m_iQuaternionSize;
		int m_iScaleSize;
		TAnimVector* m_pTranslations;
		TAnimQuaternion* m_pQuaternions;
		TAnimScale* m_pScales;
		TTRB* m_pTRB;
	};

	class TKeyframeLibraryInstance
	{
	public:
		TKeyframeLibraryInstance();

		TKeyframeLibrary* Create(TKeyframeLibrary* a_pLibrary);
		TKeyframeLibrary* CreateEx(TKeyframeLibrary* a_pLibrary, int a_iTKeyCount, int a_iQKeyCount, int a_iSKeyCount, int a_iTBaseIndex, int a_iQBaseIndex, int a_iSBaseIndex);

		void Destroy();

		TAnimVector* GetT(int a_iIndex) const
		{
			TASSERT(a_iIndex < m_iTKeyCount);
			return &m_pTranslations[a_iIndex];
		}

		TAnimQuaternion* GetQ(int a_iIndex) const
		{
			TASSERT(a_iIndex < m_iQKeyCount);
			return &m_pQuaternions[a_iIndex];
		}

		TAnimScale GetS(int a_iIndex) const
		{
			TASSERT(a_iIndex < m_iQKeyCount);
			return m_pScales[a_iIndex];
		}

		void SetLibrary(TKeyframeLibrary* a_pLibrary) { m_pLibrary = a_pLibrary; }
		void SetTCount(int a_iTCount) { m_iTKeyCount = a_iTCount; }
		void SetQCount(int a_iQCount) { m_iQKeyCount = a_iQCount; }
		void SetSCount(int a_iSCount) { m_iSKeyCount = a_iSCount; }

		TKeyframeLibrary* GetLibrary() const { return m_pLibrary; }
		int GetTCount() const { return m_iTKeyCount; }
		int GetQCount() const { return m_iQKeyCount; }
		int GetSCount() const { return m_iSKeyCount; }

	private:
		TKeyframeLibrary* m_pLibrary;
		int m_iTBaseIndex;
		int m_iQBaseIndex;
		int m_iSBaseIndex;
		int m_iTKeyCount;
		int m_iQKeyCount;
		int m_iSKeyCount;
		TAnimVector* m_pTranslations;
		TAnimQuaternion* m_pQuaternions;
		TAnimScale* m_pScales;
	};

	class TKeyframeLibraryManager
	{
	public:
		TKeyframeLibraryManager() = default;
		~TKeyframeLibraryManager() = default;

		TKeyframeLibrary* GetLibrary(const char* a_szName);

		TKeyframeLibrary* LoadLibrary(TKeyframeLibrary::TRBHeader* a_pTRBHeader);
		TKeyframeLibrary* LoadLibraryFromTRB(TTRB* a_pTRB, const char* a_szSymbolName);
		void UnloadLibrary(TKeyframeLibrary* a_pLibrary);

	private:
		TDList<TKeyframeLibrary> m_List;
		int m_iNumLibraries = 0;
	};

}
