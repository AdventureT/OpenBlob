#pragma once
#include "Toshi/Xui/TXUIButton.h"

namespace Toshi
{
	class XURXUIBackButtonData : public XURXUIButtonData
	{
	public:
		static constexpr const char* sm_sTypeInfo = "XURXUIBackButtonData";

	public:
		virtual const char* GetTypeInfo() const { return sm_sTypeInfo; }

		virtual TBOOL IsColourPropType(uint32_t a_uiObjectIndex, uint32_t propType);

		virtual TBOOL IsFloatPropType(uint32_t a_uiObjectIndex, uint32_t propType);

		virtual uint32_t GetTimelinePropSize(uint32_t a_uiObjectIndex, uint32_t propType);

		virtual TBOOL TranslateTimelineProp(const char* name, uint32_t& param_2, PropType& propType);
		virtual TBOOL ValidateTimelineProp(uint32_t a_uiObjectIndex, uint32_t param_2);

		virtual TBOOL Load(TXUIResource& resource, uint8_t*& a_pData);
	};
}


