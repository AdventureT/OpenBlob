#include "ToshiPCH.h"
#include "TXUILabel.h"

bool Toshi::XURXUILabelData::IsColourPropType(uint32_t a_uiObjectIndex, uint32_t propType)
{
    if (a_uiObjectIndex != 0) return XURXUIControlData::IsColourPropType(a_uiObjectIndex - 1, propType);
    return false;
}

bool Toshi::XURXUILabelData::IsFloatPropType(uint32_t a_uiObjectIndex, uint32_t propType)
{
    if (a_uiObjectIndex != 0) return XURXUIControlData::IsFloatPropType(a_uiObjectIndex - 1, propType);
    return false;
}

uint32_t Toshi::XURXUILabelData::GetTimelinePropSize(uint32_t a_uiObjectIndex, uint32_t propType)
{
    if (a_uiObjectIndex != 0) return XURXUIControlData::ValidateTimelineProp(a_uiObjectIndex - 1, propType);
    return 4;
}

bool Toshi::XURXUILabelData::TranslateTimelineProp(const char* name, uint32_t& param_2, PropType& propType)
{
    param_2++;
    return XURXUIControlData::TranslateTimelineProp(name, param_2, propType);
}

bool Toshi::XURXUILabelData::ValidateTimelineProp(uint32_t a_uiObjectIndex, uint32_t param_2)
{
    if (a_uiObjectIndex != 0) return XURXUIControlData::ValidateTimelineProp(a_uiObjectIndex - 1, param_2);
    return false;
}

bool Toshi::XURXUILabelData::Load(TXUIResource& resource, uint8_t*& a_pData)
{
    XURXUIControlData::Load(resource, a_pData);
    
    if (*a_pData++ != 0)
    {
        a_pData += 4;
    }
    
    return true;
}
