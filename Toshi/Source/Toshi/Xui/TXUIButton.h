#pragma once
#include "Toshi/Xui/TXUIControl.h"

namespace Toshi
{
	class XURXUIButtonData : public XURXUIControlData
	{
	public:
		static constexpr const char* sm_sTypeInfo = "XURXUIButtonData";

		enum PropType_ : PropType
		{
			PropType_PressKey,
			PropType_NUMOF
		};

	public:
		virtual const char* GetTypeInfo() const { return sm_sTypeInfo; }

		virtual TBOOL IsColourPropType(uint32_t a_uiObjectIndex, uint32_t propType);

		virtual TBOOL IsFloatPropType(uint32_t a_uiObjectIndex, uint32_t propType);

		virtual uint32_t GetTimelinePropSize(uint32_t a_uiObjectIndex, uint32_t propType);

		virtual TBOOL TranslateTimelineProp(const char* name, uint32_t& a_uiObjectIndex, PropType& propType);
		virtual TBOOL ValidateTimelineProp(uint32_t a_uiObjectIndex, uint32_t a_uiPropIndex);

		virtual TBOOL Load(TXUIResource& resource, uint8_t*& a_pData);

	protected:
		/* 0 */ uint32_t m_PressKey;
	};
}