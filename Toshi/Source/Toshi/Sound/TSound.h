#pragma once
#include <fmod/core/fmod.hpp>
#include <fmod/studio/fmod_studio.hpp>

namespace Toshi
{
	class TSound : public TSingleton<TSound>
	{
	public:

		enum SpeakerType
		{
			SpeakerType_RAW,
			SpeakerType_MONO,
			SpeakerType_7POINT1,
			SpeakerType_MAX
		};

		class SoundInitValues
		{
		public:
			SoundInitValues()
			{
				m_pPoolmem = TNULL;
				m_iPoolSize = -1;
				m_iMaxChannels = -1;
				m_ifileBufferSize = -1;
				m_eSpeakerType = SpeakerType_MAX;
			}

			void* m_pPoolmem;           // 0x0
			int m_iPoolSize;            // 0x4
			int m_iMaxChannels;         // 0x8
			int m_ifileBufferSize;      // 0xC
			SpeakerType m_eSpeakerType; // 0x10
		};


		TSound()
		{
			m_pSystem = NULL;
			m_bInitialised = TFALSE;
			m_SoundInitValues = SoundInitValues();
		}
		
		TBOOL Create(void* poolmem, int poollen, int maxchannels, int filebuffersize, SpeakerType speakerType);
		TBOOL Create(const SoundInitValues soundInitValues)
		{
			return Create(soundInitValues.m_pPoolmem, soundInitValues.m_iPoolSize, soundInitValues.m_iMaxChannels, soundInitValues.m_ifileBufferSize, soundInitValues.m_eSpeakerType);
		}

		void Update()
		{
			TASSERT(TNULL != m_pSystem);
			FMOD_RESULT eResult = m_pSystem->update();
			ErrorCheck(eResult);
		}
		
		FMOD::System* GetSystem()
		{
			TASSERT(TNULL != m_pSystem);
			return m_pSystem;
		}

		TBOOL IsInitialised() const
		{
			return m_bInitialised;
		}
	
	protected:	
		TBOOL ErrorCheck(FMOD_RESULT error);
		
		TBOOL InitMem(void* poolmem, int poollen);
		
		

	public:
		                                     // 0x0 is m_pEventSystem which is deprecated in newer FMOD
		FMOD::System* m_pSystem;             // 0x4
		TBOOL m_bInitialised;                // 0x8
		SoundInitValues m_SoundInitValues;   // 0xC
	};
}

