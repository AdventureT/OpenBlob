#pragma once

#include "Toshi/Xui/TXUIElement.h"

TOSHI_NAMESPACE_BEGIN

class TXUIVisual
{

};

class XURXUIVisualData : public XURXUIElementData
{
public:
	static constexpr const char* sm_sTypeInfo = "XURXUIVisualData";

public:
	virtual TBOOL Load(TXUIResource& resource, uint8_t*& a_pData) override;
	virtual TBOOL ValidateTimelineProp(uint32_t a_uiObjectIndex, uint32_t a_uiPropIndex) override;
	virtual TBOOL TranslateTimelineProp(const char* name, uint32_t& a_uiObjectIndex, PropType& propType) override;
	virtual uint32_t GetTimelinePropSize(uint32_t a_uiObjectIndex, uint32_t propType) override;
	virtual TBOOL IsFloatPropType(uint32_t a_uiObjectIndex, uint32_t propType) override;
	virtual TBOOL IsColourPropType(uint32_t a_uiObjectIndex, uint32_t propType) override;
	virtual const char* GetTypeInfo() const { return sm_sTypeInfo; }
};

TOSHI_NAMESPACE_END