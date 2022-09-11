#pragma once
#include "Toshi/Render/TKeyframeLibrary.h"
#include "Toshi/Core/TSystem.h"
#include "Toshi/Render/ResourceName.h"

namespace Toshi
{
	class TSkeletonSequence
	{
		enum Flag : unsigned short
		{
			FLAG_Overlay = BITFIELD(0),
		};

		enum LOOPMODE
		{
			LOOPMODE_LOOP
		};

		ResourceName m_ResourceName;
		Flag m_Flags;
		LOOPMODE m_eLoopMode;

	public:
		inline bool IsLooping() { return m_eLoopMode == LOOPMODE_LOOP; }
		inline bool IsOverlay() { return m_Flags & FLAG_Overlay; }
		inline ResourceName& GetResourceName() { return m_ResourceName; }
	};

	class TSkeletonBone
	{
		ResourceName m_ResourceName; // 0x90

	public:
		inline ResourceName& GetResourceName() { return m_ResourceName; }
	};

	class TSkeleton
	{
		// Members complete
		int m_iSize; // 0x0
		short m_sBoneCount; // 0x4
		short m_sManualBoneCount; // 0x6
		short m_sSequenceCount; // 0x8
		short m_sAnimationMaxCount; // 0xA
		short m_sInstanceCount; // 0xC

		TKeyframeLibraryInstance m_KeyLibraryInstance; // 0x10
		TSkeletonBone* m_Bones; // 0x38
		TSkeletonSequence** m_SkeletonSequences; // 0x3C

	public:

		TSkeleton();
		void Delete();
		bool Create(uint32_t param_1);

		TSkeleton& operator=(const TSkeleton& a_rSkeleton) { TIMPLEMENT(); }

		inline int GetSize() { return m_iSize; }
		inline TSkeletonBone* GetBone(int index) { return &m_Bones[index]; }
		inline TSkeletonBone* GetBone(const char* a_cBoneName, int length) { return GetBone(GetBoneID(a_cBoneName, length)); }
		int GetBoneID(const char* a_cBoneName, int length);

		inline TKeyframeLibraryInstance& GetKeyLibraryInstance() { return m_KeyLibraryInstance; }
		inline short GetAnimationMaxCount() { return m_sAnimationMaxCount; }
		inline TSkeletonBone* GetBone(int index) { return &m_Bones[index]; }
		inline TSkeletonBone* GetBones() { return m_Bones; }
		inline short GetInstanceCount() { return m_sInstanceCount; }
		inline short GetAutoBoneCount() { return m_sBoneCount - m_sManualBoneCount; }

		int GetSequenceID(const char* a_sSequenceName, int length);
		inline TSkeletonSequence** GetSequences() { return m_SkeletonSequences; }
		inline TSkeletonSequence* GetSequence(int index) { return m_SkeletonSequences[index]; }
		inline TSkeletonSequence* GetSequence(const char* a_sSequenceName, int length) { return GetSequence(GetSequenceID(a_sSequenceName, length)); }

		inline short GetSequenceCount() { return m_sSequenceCount; }
	};
}


