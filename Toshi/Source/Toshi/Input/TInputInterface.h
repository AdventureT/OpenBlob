#pragma once
#include "Toshi/Core/TNodeList.h"
#include "Toshi/Core/TArray.h"

namespace Toshi {
	
	class TInputDevice;

	class TInputInterface :
		public TGenericClassDerived<TInputInterface, TObject, "TInputInterface", TMAKEVERSION(1, 0), TFALSE>,
		public TSingleton<TInputInterface>
	{
	public:
		class InputEvent
		{
		public:
			enum EventType
			{
				EventType_StartRepeat,
				EventType_StopRepeat,
				EventType_Repeat,
				EventType_Unk3,
				EventType_MouseMotion
			};

		public:
			InputEvent() = default;

			InputEvent(TInputDevice* device, int doodad, EventType eventType)
			{
				m_pSource = device;
				m_iDoodad = doodad;
				m_eEventType = eventType;
				m_bIsMagnitudeFloat = TFALSE;
				m_iAxisCount = 0;
			}

			InputEvent(TInputDevice* device, int doodad, EventType eventType, int magnitude)
			{
				m_pSource = device;
				m_iDoodad = doodad;
				m_eEventType = eventType;
				m_Magnitude.Ints[0] = magnitude;
				m_bIsMagnitudeFloat = TFALSE;
				m_iAxisCount = 1;
			}

			InputEvent(TInputDevice* device, int doodad, EventType eventType, int magnitude, int magnitude2)
			{
				m_pSource = device;
				m_iDoodad = doodad;
				m_eEventType = eventType;
				m_Magnitude.Ints[0] = magnitude;
				m_Magnitude.Ints[1] = magnitude2;
				m_bIsMagnitudeFloat = TFALSE;
				m_iAxisCount = 2;
			}

			InputEvent(TInputDevice* device, int doodad, EventType eventType, float magnitude)
			{
				m_pSource = device;
				m_iDoodad = doodad;
				m_eEventType = eventType;
				m_Magnitude.Floats[0] = magnitude;
				m_bIsMagnitudeFloat = TTRUE;
				m_iAxisCount = 1;
			}

			InputEvent(TInputDevice* device, int doodad, EventType eventType, float magnitude, float magnitude2)
			{
				m_pSource = device;
				m_iDoodad = doodad;
				m_eEventType = eventType;
				m_Magnitude.Floats[0] = magnitude;
				m_Magnitude.Floats[1] = magnitude2;
				m_bIsMagnitudeFloat = TTRUE;
				m_iAxisCount = 2;
			}

			int8_t GetAxisCount() const
			{
				return m_iAxisCount;
			}

			int GetDoodad() const
			{
				return m_iDoodad;
			}

			EventType GetEventType() const
			{
				return m_eEventType;
			}

			TInputDevice* GetSource() const
			{
				return m_pSource;
			}

			int GetMagnitudeInt(int a_iAxis)
			{
				TASSERT(a_iAxis >= 0 && a_iAxis < GetAxisCount());

				if (IsMagnitudeInt())
				{
					return m_Magnitude.Ints[a_iAxis];
				}
				else
				{
					if (m_Magnitude.Floats[a_iAxis] < -0.5f)
					{
						return -1;
					}

					return 0.5f < m_Magnitude.Floats[a_iAxis];
				}
			}

			float GetMagnitudeFloat(int a_iAxis)
			{
				TASSERT(a_iAxis >= 0 && a_iAxis < GetAxisCount());

				if (IsMagnitudeFloat())
				{
					return m_Magnitude.Floats[a_iAxis];
				}

				return (float)m_Magnitude.Ints[a_iAxis];
			}

			TBOOL IsMagnitudeFloat()
			{
				return m_bIsMagnitudeFloat;
			}

			TBOOL IsMagnitudeInt()
			{
				return !m_bIsMagnitudeFloat;
			}

		public:
			int m_iDoodad;              // 0x0
			EventType m_eEventType;     // 0x4
			TBOOL m_bIsMagnitudeFloat;  // 0x8
			int8_t m_iAxisCount;        // 0x9

			union
			{
				float Floats[2];
				int Ints[2];
			} m_Magnitude;              // 0x10 JPOG 0x18 De blob

			TInputDevice* m_pSource;    // 0xC
		};

	public:
		TInputInterface() 
		{
			m_bIsExclusiveMode = TFALSE;
		}

		TInputDevice* GetDeviceByIndex(TClass* pClass, size_t index);

		template <class C>
		C* GetDeviceByIndex(size_t index = 0)
		{
			C* pDevice = TSTATICCAST(C*, GetDeviceByIndex(TGetClass(C), index));
			TASSERT(pDevice == TNULL || pDevice->GetClass()->IsA(TGetClass(C)));
			return pDevice;
		}

		void AddDevice(TInputDevice* device);
		void RemoveDevice(TInputDevice* device);

		virtual TBOOL Initialise() { return TTRUE; }
		virtual TBOOL Deinitialise();

		virtual TBOOL AcquireAll();
		virtual TBOOL UnacquireAll();
		virtual TBOOL FlushAll();
		virtual void SetExclusiveMode(TBOOL mode);
		virtual TBOOL GetExclusiveMode() const;
		virtual int ProcessEvents(float fUnk);
		virtual void StopAllRepeats();

	private:
		TNodeList<TInputDevice> m_DeviceList;                                        // 0x04
		TBOOL m_bIsExclusiveMode;                                                    // 0x14 
		TEmitter<TInputInterface, TInputInterface::InputEvent> m_Emitter1;           // 0x24
		TGenericEmitter m_Emitter2;                                                  // 0x28
	};

	class TInputDevice :
		public TGenericClassDerived<TInputDevice, TObject, "TInputDevice", TMAKEVERSION(1, 0), TFALSE>,
		public TNodeList<TInputDevice>::TNode
	{
	public:
		static constexpr int INPUT_DEVICE_MOUSE_BUTTONS = 3;
		static constexpr int INPUT_DEVICE_MOUSE_WHEEL = 4;

		struct DoodadProperties
		{
			int m_iUnk;
			TBOOL m_bFlag;
		};

		struct RepeatInfo
		{
			int m_iDoodad;
		};

		static constexpr size_t MAX_DEVICE_COUNT = 14;
		inline static TInputDevice* s_aDevices[MAX_DEVICE_COUNT];
		inline static size_t s_uiDeviceCount;

	public:
		TInputDevice()
		{
			TIMPLEMENT();
			m_pInputInterface = TNULL;
			m_bIsAcquired = TFALSE;
			m_uiDeviceIndex = s_uiDeviceCount++;
			s_aDevices[m_uiDeviceIndex] = this;
		}

		virtual TBOOL Flush() { return TTRUE; }
		virtual TBOOL StartRepeat(int param_1, float param_2, float param_3);
		virtual TBOOL StopRepeat(int param_1);
		virtual TBOOL StopAllRepeats();
		virtual void ThrowRepeatEvent(TEmitter<TInputInterface, TInputInterface::InputEvent>& emitter, RepeatInfo* repeatInfo, float deltaTime);
		virtual TBOOL IsForceFeedbackDevice() { return TFALSE; }
		virtual int ProcessEvents(TEmitter<TInputInterface, TInputInterface::InputEvent>& emitter, float deltaTime) { return 0; };
		virtual void Update(float deltaTime) { };
		virtual TBOOL Acquire() { return TFALSE; };
		virtual TBOOL Unacquire() { return TFALSE; };
		virtual TBOOL IsDown(int doodad) const { return TFALSE; }
		virtual TBOOL WasDown(int doodad) const { return TFALSE; }

		TBOOL IsAcquired() const
		{
			return m_bIsAcquired;
		}

		TInputInterface* GetInputInterface()
		{
			return m_pInputInterface;
		}

		void SetInputInterface(TInputInterface* a_pInterface)
		{
			m_pInputInterface = a_pInterface;
		}

	protected:
		int ProcessRepeats(TEmitter<TInputInterface, TInputInterface::InputEvent>& emitter, float flt);

	protected:
		size_t m_uiDeviceIndex;             // 0x14
		TArray<void*>::Storage m_Array1;    // 0x18 FIXME: replace void* with some structure which size is 0xC
		TArray<void*>::Storage m_Array2;    // 0x28 FIXME: replace void* with some structure which size is 0xC
		TBOOL m_bUnknown;                   // 0x38
		TBOOL m_bIsAcquired;                // 0x39 de blob 0x35 JPOG
		TInputInterface* m_pInputInterface; // 0x3C
	};

}
