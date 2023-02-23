#include "ToshiPCH.h"
#include "TXUIElement.h"
#include "Toshi/Xui/TXUI.h"
#include "Toshi/Xui/TXUIListItem.h"
#include "XURReader.h"

namespace Toshi
{
	bool XURXUIObjectData::Load(TXUIResource& resource, uint8_t*& a_pData)
	{
		XURReader reader(a_pData);
		m_Index = reader.ReadUInt16();
		return true;
	}

	void XURXUIObjectData::LoadChildren(TXUIResource& resource, uint8_t*& a_pData)
	{
		XURReader reader(a_pData);
		m_NumChildren = reader.ReadUInt8From32();
		m_Children = new (TXUI::MemoryBlock()) XURXUIObjectData*[m_NumChildren];
		TASSERT(m_Children != TNULL);

		for (size_t i = 0; i < m_NumChildren; i++)
		{
			uint16_t objectIndex = reader.ReadUInt16();

			m_Children[i] = TXUIResource::CreateObjectData(resource, objectIndex);
			m_Children[i]->m_Index2 = objectIndex;
			uint8_t opcode = reader.ReadUInt8();

			m_Children[i]->Load(resource, a_pData);

			if (opcode & 2)
			{
				m_Children[i]->LoadChildren(resource, a_pData);

				if (m_Children[i]->LoadNamedFrames(resource, a_pData), opcode & 4)
				{
					m_Children[i]->LoadTimelines(resource, a_pData);
				}
			}
		}
	}

	bool XURXUIObjectData::LoadNamedFrames(TXUIResource& resource, uint8_t*& a_pData)
	{
		XURReader reader(a_pData);
		m_NumNamedFrames = reader.ReadEPTUShort32();
		TASSERT(m_NumNamedFrames < 1000, "Max of named Frames reached");

		if (m_NumNamedFrames != 0)
		{
			m_pNamedFrames = new (TXUI::MemoryBlock()) XURXUINamedFrameData[m_NumNamedFrames];
			TASSERT(m_pNamedFrames != TNULL);

			for (size_t i = 0; i < m_NumNamedFrames; i++)
			{
				m_pNamedFrames[i].m_unk2 = reader.ReadUInt16();
				m_pNamedFrames[i].m_unk = reader.ReadUInt16From32();
				m_pNamedFrames[i].m_unk4 = reader.ReadUInt8();
				m_pNamedFrames[i].m_unk3 = reader.ReadUInt16();
			}
		}

		return true;
	}

	void XURXUIObjectData::LoadTimelines(TXUIResource& resource, uint8_t*& a_pData)
	{
		XURReader reader(a_pData);
		m_NumTimelines = reader.ReadUInt8From32();

		if (m_NumTimelines != 0)
		{
			m_TimelinesData = new (TXUI::MemoryBlock()) XURXUITimelineData[m_NumTimelines];
			TASSERT(m_TimelinesData != TNULL);

			for (size_t i = 0; i < m_NumTimelines; i++)
			{
				//m_timelinesData[i] = *this;
				m_TimelinesData[i].Load(resource, a_pData);
			}
		}
	}

	XURXUIElementData* XURXUIObjectData::FindChildElementData(uint32_t index)
	{
		for (size_t i = 0; i < m_NumChildren; i++)
		{
			XURXUIElementData* pElementData = static_cast<XURXUIElementData*>(m_Children[m_NumChildren]);

			if (pElementData->GetId() == index)
			{
				return pElementData;
			}
		}

		return TNULL;
	}

	bool XURXUIElementData::Load(TXUIResource& resource, uint8_t*& a_pData)
	{
		XURXUIObjectData::Load(resource, a_pData);
	
		if (*a_pData++ != 0)
		{
			XURReader reader(a_pData);
			if (m_Index != 0) reader.ReadPropsInfo<PropType_NUMOF>();

			XUIEPTFloat width, height, opacity;
			XUIEPTUShort32 anchor, blendMode;
			XUIEPTBool show, disableTimelineRecursion, clipChildren, designTime;
			XUIEPTUnsigned colorWriteFlags;

			bool hasId = reader.ReadProperty<XUI_EPT_STRING>(PropType_Id, m_Id);
			bool hasWidth = reader.ReadProperty<XUI_EPT_FLOAT>(PropType_Width, width);
			bool hasHeight = reader.ReadProperty<XUI_EPT_FLOAT>(PropType_Height, height);
			bool hasPosition = reader.ReadProperty<XUI_EPT_VECTOR>(PropType_Position, m_Position);
			bool hasScale = reader.ReadProperty<XUI_EPT_VECTOR>(PropType_Scale, m_Scale);
			bool hasRotation = reader.ReadProperty<XUI_EPT_QUATERNION>(PropType_Rotation, m_Rotation);
			bool hasOpacity = reader.ReadProperty<XUI_EPT_FLOAT>(PropType_Opacity, opacity);
			bool hasAnchor = reader.ReadProperty<XUI_EPT_USHORT32>(PropType_Anchor, anchor);
			bool hasPivot = reader.ReadProperty<XUI_EPT_VECTOR>(PropType_Pivot, m_Pivot);
			bool hasShow = reader.ReadProperty<XUI_EPT_BOOL>(PropType_Show, show);
			bool hasBlendMode = reader.ReadProperty<XUI_EPT_USHORT32>(PropType_BlendMode, blendMode);
			bool hasDisableTimelineRecursion = reader.ReadProperty<XUI_EPT_BOOL>(PropType_DisableTimelineRecursion, disableTimelineRecursion);
			bool hasDesignTime = reader.ReadProperty<XUI_EPT_BOOL>(PropType_DesignTime, designTime);
			bool hasColorWriteFlags = reader.ReadProperty<XUI_EPT_UNSIGNED>(PropType_ColorWriteFlags, colorWriteFlags);;
			bool hasClipChildren = reader.ReadProperty<XUI_EPT_BOOL>(PropType_ClipChildren, clipChildren);
			
			if (hasWidth)
			{
				m_Width = T2GUIElement::PackFloat(width);
			}

			if (hasHeight)
			{
				m_Height = T2GUIElement::PackFloat(height);
			}

			if (hasOpacity)
			{
				TASSERT(opacity >= 0.0f && opacity <= 1.0f);
				m_Opacity = opacity * 255;
			}

			if (hasAnchor)
			{
				TASSERT(anchor <= Flags_AnchorMask);
				m_Flags |= reader.ReadEPTUShort32() << 3;
			}

			if (hasShow)
			{
				if (show)
				{
					m_Flags |= 0x1000;
				}
				else
				{
					m_Flags &= ~0x1000;
				}
			}

			if (hasBlendMode)
			{
				m_Flags |= blendMode;
			}

			if (hasDisableTimelineRecursion)
			{
				if (disableTimelineRecursion)
				{
					m_Flags = 0x2000;
				}
				else
				{
					m_Flags &= ~0x2000;
				}
			}
			
			if (hasClipChildren)
			{
				if (clipChildren)
				{
					m_Flags |= 0x800;
				}
				else
				{
					m_Flags &= ~0x800;
				}
			}
		}

		return true;
	}

	bool XURXUIElementData::TranslateTimelineProp(const char* name, uint32_t& param_2, PropType& propType) // param_3 = position?
	{
		TXUI_TRANSLATE_TIMELINE_PROP(name, Id, propType);
		TXUI_TRANSLATE_TIMELINE_PROP(name, Width, propType);
		TXUI_TRANSLATE_TIMELINE_PROP(name, Height, propType);
		TXUI_TRANSLATE_TIMELINE_PROP(name, Position, propType);
		TXUI_TRANSLATE_TIMELINE_PROP(name, Scale, propType);
		TXUI_TRANSLATE_TIMELINE_PROP(name, Rotation, propType);
		TXUI_TRANSLATE_TIMELINE_PROP(name, Opacity, propType);
		TXUI_TRANSLATE_TIMELINE_PROP(name, Anchor, propType);
		TXUI_TRANSLATE_TIMELINE_PROP(name, Pivot, propType);
		TXUI_TRANSLATE_TIMELINE_PROP(name, Show, propType);
		TXUI_TRANSLATE_TIMELINE_PROP(name, BlendMode, propType);
		TXUI_TRANSLATE_TIMELINE_PROP(name, DisableTimelineRecursion, propType);
		TXUI_TRANSLATE_TIMELINE_PROP(name, ColorWriteFlags, propType);
		TXUI_TRANSLATE_TIMELINE_PROP(name, ClipChildren, propType);

		return false;
	}

	bool XURXUIElementData::ValidateTimelineProp(uint32_t a_uiObjectIndex, uint32_t param_2)
	{
		TASSERT(a_uiObjectIndex == 0);
		return param_2 < PropType_NUMOF;
	}

	// TXUIElement

	TXUIElement::TXUIElement()
	{
		m_pObjectData = TNULL;
		m_Flags1 |= FLAGS_XUIELEMENT;
		m_vRotation = T2GUITransform::Rotation(0,0);
		m_vPosition = T2GUITransform::Rotation(0, 0);
		m_vScale = T2GUITransform::Rotation(0, 0);
	}

	bool TXUIElement::SkipRender()
	{
		TIMPLEMENT();
		return false;
	}

	void TXUIElement::SetHeight(float height)
	{
		TVector2 currentDimension;
		GetDimensions(currentDimension.x, currentDimension.y);

		T2GUIElement::SetHeight(height);

		TVector2 newDimension;
		GetDimensions(newDimension.x, newDimension.y);

		if (!currentDimension.IsEqual(newDimension))
		{
			TVector2 combined = currentDimension - newDimension;
			UpdateAnchoring(combined);
		}
	}

	void TXUIElement::SetWidth(float width)
	{
		TVector2 currentDimension;
		GetDimensions(currentDimension.x, currentDimension.y);

		T2GUIElement::SetWidth(width);

		TVector2 newDimension;
		GetDimensions(newDimension.x, newDimension.y);

		if (!currentDimension.IsEqual(newDimension))
		{
			TVector2 combined = currentDimension - newDimension;
			UpdateAnchoring(combined);
		}
	}

	void TXUIElement::UpdateAnchoring(const TVector2& vec)
	{
		TIMPLEMENT();
	}

	bool TXUIElement::IsVisible()
	{
		if (HASFLAG(m_Flags1 & FLAGS_VISIBLE) && HASFLAG(m_Flags1 & (uint8_t)T2GUIElement::s_uiGlobalVisMask))
		{
			return TFALSE;
		}

		return TTRUE;
	}

	bool TXUIElement::Create(TXUIResource& a_rResource, XURXUIElementData* a_pElementData, bool hasChildren)
	{
		m_pObjectData = a_pElementData;
		
		if (GetClass() == TGetClass(TXUICanvas))
		{
			a_rResource.PushID(a_rResource.GetString(a_pElementData->GetId()));
		}

		m_objectID = a_rResource.GetString(a_pElementData->GetId());
		if (TXUIResource::s_bGenerateUIDs && TStringManager::String16Length(m_objectID) != 0)
		{
			if (GetClass()->IsA(TGetClass(TXUIListItem)))
			{
				m_iUIDCount = TXUIResource::s_iUIDCount;
				TXUIResource::s_iUIDCount++;
			}
		}

		m_Width = a_pElementData->GetWidth();
		m_Height = a_pElementData->GetHeight();

		if (a_pElementData->GetPosition() != -1)
		{
			TVector4* pos = a_rResource.GetVector(a_pElementData->GetPosition());
			
			m_vPosition.SetX(pos->x);
			m_vPosition.SetY(pos->x);
		}
		
		if (a_pElementData->GetRotation() != -1)
		{
			//m_Rotation = a_rResource.GetQuat(a_pObjectData->m_Scale);
		}

		if (a_pElementData->GetScale() != -1)
		{
			TVector4* scale = a_rResource.GetVector(a_pElementData->GetScale());
			m_vScale.SetX(scale->x);
			m_vScale.SetY(scale->x);
		}

		SetVisible(a_pElementData->IsVisible());

		if (hasChildren)
		{
			CreateChildren(a_rResource, a_pElementData);
		}

		if (GetClass() == TGetClass(TXUICanvas))
		{
			a_rResource.PopID();
		}

		return true;
	}

	void TXUIElement::CreateChildren(TXUIResource& a_rResource, XURXUIElementData* a_pElementData)
	{
		for (size_t i = 0; i < a_pElementData->m_NumChildren; i++)
		{
			auto child = a_pElementData->m_Children[i];
		}

		TIMPLEMENT();
	}
}
