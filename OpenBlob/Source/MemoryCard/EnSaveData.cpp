#include "pch.h"
#include "EnSaveData.h"

bool EnSaveData::Validate()
{
    int result = Toshi2::T2String8::CompareStrings(m_savedata->header.m_id, "BB", 2);

    if (result == 0 && m_savedata->header.m_size == 3208)
    {
        uint32_t prevCRC = m_savedata->header.m_crc;
        m_savedata->header.m_crc = 0;
        m_savedata->header.m_crc = Toshi::TUtil::CRC32(m_buffer, m_size);

        if (m_savedata->header.m_crc != prevCRC) return false;

        float fVal1 = 0.5f;
        float fVal2 = 1.0f;

    }
    return false;
}
