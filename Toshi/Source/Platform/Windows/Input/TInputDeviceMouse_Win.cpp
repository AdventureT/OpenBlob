#include "ToshiPCH.h"
#include "TInputDeviceMouse_Win.h"
#include <Platform/Windows/DX11/TRender_DX11.h>
#include <Platform/Windows/Input/TInputInterface_Win.h>

namespace Toshi {

    void TInputDXDeviceMouse::Release()
    {
        if (m_poDXInputDevice)
        {
            m_poDXInputDevice->Release();
            m_poDXInputDevice = NULL;
        }
    }

    void TInputDXDeviceMouse::Update()
    {
        if (m_bUnk)
        {
            m_aAxis.m_iX = 0;
            m_aAxis.m_iY = 0;
            m_fWheelAxis = 0;
            auto renderer = TRenderDX11::Interface();
            if (renderer != TNULL)
            {
                auto params = renderer->GetCurrentDisplayParams();
                if (params->Height == 0)
                {
                    TTODO("2x FUN_00667490");
                }
                TMSWindow* window = renderer->GetMSWindow();
                m_CursorPos.x = window->m_xPos;
                m_CursorPos.y = window->m_yPos;
            }
        }
    }

    TBOOL TInputDXDeviceMouse::Initialise()
    {
        TIMPLEMENT();

        m_field0x40 = 0;
        m_dwButtonCurrent = 0;
        m_dwButtonPrevious = 0;

        GetCursorPos(&m_CursorPos);

        DIPROPDWORD dwordProperty{};
        dwordProperty.diph.dwSize = sizeof(dwordProperty);
        dwordProperty.diph.dwHeaderSize = sizeof(DIPROPHEADER);
        dwordProperty.diph.dwObj = DIMOFS_Z;
        dwordProperty.diph.dwHow = DIPH_BYOFFSET;
    
        HRESULT hr = m_poDXInputDevice->GetProperty(DIPROP_GRANULARITY, &dwordProperty.diph);
        if (hr != DIERR_OBJECTNOTFOUND)
        {
            m_bInitiliased = hr == DI_OK;
            return TTRUE;
        }
        m_bInitiliased = TFALSE;
        return TTRUE;
    }

    TBOOL TInputDXDeviceMouse::Deinitialise()
    {
        Release();
        return TTRUE;
    }

    TBOOL TInputDXDeviceMouse::Acquire()
    {
        HRESULT hr = m_poDXInputDevice->Acquire();
        if (FAILED(hr))
        {
            m_bIsAquired = TFALSE;
            return TFALSE;
        }

        m_bIsAquired = TTRUE;

        if (hr != S_FALSE)
        {
            RefreshDirect();
        }

        return TFALSE;
    }

    TBOOL TInputDXDeviceMouse::Unacquire()
    {
        m_bIsAquired = TFALSE;
        HRESULT hr = m_poDXInputDevice->Unacquire();
        return SUCCEEDED(hr);
    }

    TBOOL TInputDXDeviceMouse::Flush()
    {
        DWORD dwItems = INFINITE;
        if (m_bIsAquired)
        {
            HRESULT hr = m_poDXInputDevice->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), NULL, &dwItems, 0);
            if (SUCCEEDED(hr))
            {
                RefreshDirect();
                return TTRUE;
            }
        }
        return TFALSE;
    }

    int TInputDXDeviceMouse::ProcessEvents(TEmitter<TInputInterface, TInputInterface::InputEvent>& emitter, float flt)
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
                return TFALSE;
            }

        }

        auto renderer = TRenderDX11::Interface();
        auto input = TInputDXInterface::GetInterface();
        TRenderDX11::DisplayParams* params;
        int unk;

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
                    if (!params->Unk5)
                    {
                        m_CursorPos.x += dod[i].dwData;
                    }
                }
                emitter.Throw(TInputInterface::InputEvent(this, AXIS_CURSOR, TInputInterface::InputEvent::EventType_MouseMotion, m_aAxis.m_iX, m_aAxis.m_iY));
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
                    if (!params->Unk5)
                    {
                        m_CursorPos.y += dod[i].dwData;
                    }
                }
                emitter.Throw(TInputInterface::InputEvent(this, AXIS_CURSOR, TInputInterface::InputEvent::EventType_MouseMotion, m_aAxis.m_iX, m_aAxis.m_iY));
                break;
            case DIMOFS_Z: // Wheel
                m_fWheelAxis += dod[i].dwData;
                emitter.Throw(TInputInterface::InputEvent(this, AXIS_WHEEL, TInputInterface::InputEvent::EventType_MouseMotion, 0.0f, m_fWheelAxis / WHEEL_DELTA));
                unk = m_fWheelAxis / m_field0x80;
                if (unk < 0)
                {
                    emitter.Throw(TInputInterface::InputEvent(this, BUTTON_WHEEL_BACKWARD, TInputInterface::InputEvent::EventType_Unk));
                    unk++;
                    if (unk < 0)
                    {
                        int i = -unk;
                        do
                        {
                            emitter.Throw(TInputInterface::InputEvent(this, BUTTON_WHEEL_BACKWARD, TInputInterface::InputEvent::EventType_Repeat));
                            i--;
                        } while (i != 0);
                    }

                    emitter.Throw(TInputInterface::InputEvent(this, BUTTON_WHEEL_BACKWARD, TInputInterface::InputEvent::EventType_Unk2));
                }
                TTODO("...");
                break;
            case DIMOFS_BUTTON0:
            case DIMOFS_BUTTON1:
                TTODO("...");
                break;
            default:
                break;
            }
        }

        return 0;
    }

    void TInputDXDeviceMouse::RefreshDirect()
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

    TBOOL const TInputDXDeviceMouse::BindToDIDevice(HWND a_mainWindow, LPCDIDEVICEINSTANCE a_poDeviceInstance, IDirectInputDevice8* a_poDXInputDevice, TBOOL exclusive)
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
            return TFALSE;
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
        dipdw.dwData = DIPROPAXISMODE_REL;

        hr = m_poDXInputDevice->SetProperty(DIPROP_AXISMODE, &dipdw.diph);

        return hr == DI_OK;
    }

    int TInputDXDeviceMouse::GetAxisInt(int doodad, Coord coord) const
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

    float TInputDXDeviceMouse::GetAxisFloat(int doodad, Coord coord) const
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

}
