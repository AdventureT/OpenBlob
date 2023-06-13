#pragma once
#include "T2GUIElement.h"
#include "T2GUIMaterial.h"
#include "Toshi/Render/TTexture.h"

namespace Toshi
{
	class T2GUIRenderer
	{
	public:
		T2GUIRenderer() = default;
		virtual ~T2GUIRenderer() = default;

		virtual TTexture* GetTexture(const char* texName) const;
		virtual void BeginScene() = 0;
		virtual void EndScene() = 0;
		virtual void SetMaterial(T2GUIMaterial* pMat) = 0;
		virtual void PushTransform(const T2GUITransform& transform, const TVector2& vec1, const TVector2& vec2) = 0;
		virtual void PopTransform() = 0;
		virtual void SetTransform(const T2GUITransform& transform) = 0;
		virtual void SetColour(uint32_t colour) = 0;
		virtual void RenderRectangle(const TVector2& a, const TVector2& b, const TVector2& uv1, const TVector2& uv2) = 0;
		virtual void RenderTriStrip(TVector2* vertices, TVector2* UV, uint32_t numverts, float fPosScaleX, float fPosScaleY) = 0;
		virtual void RenderLine(const TVector2& a, const TVector2& b) = 0;
		virtual void RenderLine(float x1, float y1, float x2, float y2) = 0;
		virtual void RenderOutlineRectangle(const TVector2& a, const TVector2& b) = 0;
		virtual void RenderFilledRectangle(const TVector2& a, const TVector2& b) = 0;
		virtual void RenderIndexedTriList(TVector2* pVertices, TVector2* pUV, void* pIndices, uint32_t numindices, int indexSize, uint32_t numverts, float fPosScaleX, float fPosScaleY) = 0;
		virtual void RenderIndexedTriStripColoursList(TVector2* pVertices, TVector2* pUV, uint32_t* pColors, void* pIndices, uint32_t numindices, int indexSize, uint32_t numverts, float fPosScaleX, float fPosScaleY) = 0;
		virtual void ScaleCoords(float& x, float& y) = 0;
		virtual void SetScissors(float f1, float f2, float f3, float f4) = 0;
		virtual void ClearScissors() = 0;

	private:
		int m_WhiteGlyphsCount;
	};
}