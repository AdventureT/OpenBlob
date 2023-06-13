#pragma once
#include "Toshi/Render/TMaterial.h"
#include "Platform/Windows/DX11/TTexture_DX11.h"

namespace Toshi
{
	class T2GUIMaterial : public TMaterial
	{
	public:

		enum BLENDMODE
		{
			BLENDMODE_NORMAL,
			BLENDMODE_ADD,
			BLENDMODE_SUBSTRACT,
			BLENDMODE_MAX
		};

		T2GUIMaterial();

		void SetTexture(TTexture* pTexture)
		{
			m_pTexture = pTexture;
		}

		TTexture* GetTexture()
		{
			return m_pTexture;
		}

		void SetSamplerId(int iSamplerId)
		{
			m_iSamplerId = iSamplerId;
		}

		int GetSamplerId() const
		{
			return m_iSamplerId;
		}

	private:
		TTexture* m_pTexture;   // 0x54
		int m_iSamplerId;       // 0x5C
	};
}