#pragma once
#include "TXUIControl.h"

TOSHI_NAMESPACE_BEGIN

class TXUICheckBox :
	public TGenericClassDerived<TXUICheckBox, TXUIControl, "TXUICheckBox", TMAKEVERSION(1, 0), TFALSE>
{
};

class XURXUICheckBoxData : public XURXUIControlData
{

	TXUI_TYPEINFO(XURXUICheckBoxData)

	enum PropType_ : PropType
	{
		PropType_PressKey,
		PropType_NUMOF,
	};

public:

	XURXUICheckBoxData() : XURXUIControlData()
	{
		m_pClass = TFindClass(TXUICheckBox, TXUIControl);
	}

public:
	virtual TBOOL Load(TXUIResource& resource, uint8_t*& a_pData) override;
	virtual TBOOL ValidateTimelineProp(uint32_t a_uiObjectIndex, uint32_t a_uiPropIndex) override;
	virtual TBOOL TranslateTimelineProp(const char* name, uint32_t& a_uiObjectIndex, PropType& propType) override;
	virtual uint32_t GetTimelinePropSize(uint32_t a_uiObjectIndex, uint32_t propType) override;
	virtual TBOOL IsFloatPropType(uint32_t a_uiObjectIndex, uint32_t propType) override;
	virtual TBOOL IsColourPropType(uint32_t a_uiObjectIndex, uint32_t propType) override;

private:
	XUIEPTUnsigned m_uiPressKey;
};

TOSHI_NAMESPACE_END