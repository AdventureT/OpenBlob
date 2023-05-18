#include "ToshiPCH.h"
#include "TInputDeviceMouse_Win.h"
#include <Platform/Windows/DX11/TRender_DX11.h>
#include <Platform/Windows/Input/TInputInterface_Win.h>

void Toshi::TInputDXDeviceMouse::Release()
{
	if (m_poDXInputDevice)
	{
		m_poDXInputDevice->Release();
		m_poDXInputDevice = NULL;
	}
}

bool Toshi::TInputDXDeviceMouse::Initialise()
{
	TIMPLEMENT();

	m_field0x40 = 0;
	m_dwButtonCurrent = 0;
	m_dwButtonPrevious = 0;

	GetCursorPos(&m_CursorPos);

	DIPROPDWORD dwordProperty{};
	dwordProperty.diph.dwSize = sizeof(dwordProperty);
	dwordProperty.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dwordProperty.diph.dwObj = 8;
	dwordProperty.diph.dwHow = DIPH_BYOFFSET;

	HRESULT hr = m_poDXInputDevice->GetProperty(DIPROP_GRANULARITY, &dwordProperty.diph);

	TTODO("Which DIERR is 0x80070020?");
	if (hr != 0x80070020)
	{
		m_bInitiliased = hr == DI_OK;
		return m_bInitiliased;
	}
	m_bInitiliased = false;
	return true;
}

bool Toshi::TInputDXDeviceMouse::Deinitialise()
{
	Release();
	return true;
}

bool Toshi::TInputDXDeviceMouse::Acquire()
{
	HRESULT hr = m_poDXInputDevice->Acquire();
	if (FAILED(hr))
	{
		m_bIsAquired = false;
		return false;
	}

	m_bIsAquired = true;

	if (hr != S_FALSE)
	{
		RefreshDirect();
	}

	return false;
}

bool Toshi::TInputDXDeviceMouse::Unacquire()
{
	m_bIsAquired = false;
	HRESULT hr = m_poDXInputDevice->Unacquire();
	return SUCCEEDED(hr);
}

bool Toshi::TInputDXDeviceMouse::Flush()
{
	DWORD dwItems = INFINITE;
	if (m_bIsAquired)
	{
		HRESULT hr = m_poDXInputDevice->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), NULL, &dwItems, 0);
		if (SUCCEEDED(hr))
		{
			RefreshDirect();
			return true;
		}
	}
	return false;
}

int Toshi::TInputDXDeviceMouse::ProcessEvents(TGenericEmitter& emitter, float flt)
{
	DIDEVICEOBJECTDATA dod[32];
	DWORD dwItems = 32;

	TUtil::MemClear(dod, sizeof(dod));

	HRESULT hr = m_poDXInputDevice->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), dod, &dwItems, 0);

	if (FAILED(hr) && hr == (DIERR_NOTACQUIRED | DIERR_INPUTLOST))
	{
		Unacquire();
		Acquire();

		hr = m_poDXInputDevice->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), dod, &dwItems, 0);

		if (hr != DI_OK)
		{
			return false;
		}

	}

	auto renderer = TRenderDX11::Interface();
	auto input = TInputDXInterface::GetInterface();
	TRenderDX11::DisplayParams* params;
	TInputDXInterface::InputEvent inputEvent;

	for (size_t i = 0; i < dwItems; i++)
	{
		
		switch (dod[i].dwOfs)
		{
		case DIMOFS_X: // MouseUp
			m_aAxis.m_iX += dod[i].dwData;

			if (renderer != TNULL)
			{
				m_CursorPos.x += dod[i].dwData;
			}
			else
			{
				params = renderer->GetCurrentDisplayParams();
				TTODO("Something with displayparams");
			}
			inputEvent = TInputDXInterface::InputEvent(this, AXIS_CURSOR, TInputDXInterface::InputEvent::EventType_MouseMotion, m_aAxis.m_iX, m_aAxis.m_iY);
			emitter.Throw(&inputEvent);
			break;
		case DIMOFS_Y: // MouseDown
			m_aAxis.m_iY += dod[i].dwData;

			if (renderer != TNULL)
			{
				m_CursorPos.y += dod[i].dwData;
			}
			else
			{
				params = renderer->GetCurrentDisplayParams();
				TTODO("Something with displayparams");
			}
			inputEvent = TInputDXInterface::InputEvent(this, AXIS_CURSOR, TInputDXInterface::InputEvent::EventType_MouseMotion, m_aAxis.m_iX, m_aAxis.m_iY);
			emitter.Throw(&inputEvent);

			break;
		case DIMOFS_Z: // Wheel
			inputEvent = TInputDXInterface::InputEvent(this, AXIS_WHEEL, TInputDXInterface::InputEvent::EventType_Unk, m_fWheelAxis / 120.0f);
			emitter.Throw(&inputEvent);
			break;
		default:
			break;
		}
	}



	return 0;
}

void Toshi::TInputDXDeviceMouse::RefreshDirect()
{
	if (m_bIsAquired)
	{
		HRESULT hr = m_poDXInputDevice->Poll();
		if (SUCCEEDED(hr))
		{
			DIMOUSESTATE mouseState;
			hr = m_poDXInputDevice->GetDeviceState(sizeof(DIMOUSESTATE), &mouseState);
			if (SUCCEEDED(hr))
			{
				if (mouseState.rgbButtons[0] & 0x80) // LeftClick
				{
					if (m_field0x40 == 0)
					{
						m_dwButtonCurrent |= 1;
					}
					else
					{
						m_dwButtonCurrent |= INPUT_DEVICE_MOUSE_BUTTONS;
					}
				}
				if (mouseState.rgbButtons[1] & 0x80) // RightClick
				{
					if (m_field0x40 == 0)
					{
						m_dwButtonCurrent |= INPUT_DEVICE_MOUSE_BUTTONS;
					}
					else
					{
						m_dwButtonCurrent |= 1;
					}
				}
				if (mouseState.rgbButtons[2] & 0x80) // Wheeling yeeet
				{
					m_dwButtonCurrent |= INPUT_DEVICE_MOUSE_WHEEL;
				}
			}
		}
	}
}

bool const Toshi::TInputDXDeviceMouse::BindToDIDevice(HWND a_mainWindow, LPCDIDEVICEINSTANCE a_poDeviceInstance, IDirectInputDevice8* a_poDXInputDevice, bool exclusive)
{
	TASSERT(a_poDeviceInstance != NULL);
	TASSERT(a_poDXInputDevice != NULL);

	Release();

	TIMPLEMENT_D("Weird for loop");

	m_poDXInputDevice = a_poDXInputDevice;
	m_DIDevCaps.dwSize = sizeof(DIDEVCAPS);

	m_poDXInputDevice->GetCapabilities(&m_DIDevCaps);
	HRESULT hr = m_poDXInputDevice->SetDataFormat(&c_dfDIMouse);

	if (hr != DI_OK)
	{
		return false;
	}

	if (a_mainWindow)
	{
		m_poDXInputDevice->SetCooperativeLevel(a_mainWindow, DISCL_FOREGROUND | (exclusive ? DISCL_EXCLUSIVE : DISCL_NONEXCLUSIVE));
	}

	DIPROPDWORD dipdw{};
	dipdw.diph.dwSize = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwObj = 0;
	dipdw.diph.dwHow = DIPH_DEVICE;
	dipdw.dwData = 1;

	m_poDXInputDevice->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);

	return true;
}

int Toshi::TInputDXDeviceMouse::GetAxisInt(int doodad, Coord coord) const
{
	if (doodad == 0x3000b)
	{
		if (coord == X)
		{
			return m_aAxis.m_iX;
		}
		if (coord == Y)
		{
			return m_aAxis.m_iY;
		}
	}
	return 0;
}

float Toshi::TInputDXDeviceMouse::GetAxisFloat(int doodad, Coord coord) const
{
	if (doodad == 0x3000b)
	{
		if (coord == X)
		{
			return m_aAxis.m_fX;
		}
		if (coord == Y)
		{
			return m_aAxis.m_fY;
		}
	}
	else if (doodad == 0x3000c)
	{
		return m_fWheelAxis;
	}
	return 0;
}
