#pragma once
#include <Toshi/Xui/TXUIResource.h>
#include "TXUIElement.h"

namespace Toshi
{
	class XURXUIStrokeData : public XURXUIObjectData
	{
		static constexpr const char* sm_sTypeInfo = "XURXUIStrokeData";

		/* 0 */ float m_strokeWidth;
		/* 1 */ uint32_t m_strokeColor;

	public:

		virtual const char* GetTypeInfo() const { return sm_sTypeInfo; }

		virtual bool IsColourPropType(uint32_t propType);

		virtual bool IsFloatPropType(uint32_t propType);

		virtual uint32_t GetTimelinePropSize(uint32_t propType);

		virtual bool TranslateTimelineProp(const char* param_1, uint32_t& param_2, uint32_t& param_3);
		virtual bool ValidateTimelineProp(uint32_t param_2);

		virtual bool Load(TXUIResource& resource, uint8_t*& a_pData);
	};
}