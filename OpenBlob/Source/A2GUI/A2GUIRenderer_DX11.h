#pragma once
#include <Toshi2/T2GUI/T2GUIRenderer.h>
#include <Toshi/Render/TTexture.h>

class A2GUIRenderer : public Toshi::T2GUIRenderer
{
public:
	static constexpr size_t TRANSFORMSTACKSIZE = 32;

public:
	A2GUIRenderer();
	~A2GUIRenderer();

	virtual Toshi::TTexture* GetTexture(const char* texName) const override;
	virtual void BeginScene() override;
	virtual void EndScene() override;
	virtual void SetMaterial(Toshi::T2GUIMaterial* pMat) override;
	virtual void PushTransform(const Toshi::T2GUITransform& transform, const Toshi::TVector2& vec1, const Toshi::TVector2& vec2) override;
	virtual void PopTransform() override;
	virtual void SetTransform(const Toshi::T2GUITransform& transform) override;
	virtual void SetColour(uint32_t colour) override;
	virtual void RenderRectangle(const Toshi::TVector2& a, const Toshi::TVector2& b, const Toshi::TVector2& uv1, const Toshi::TVector2& uv2) override;
	virtual void RenderTriStrip(void* unk1, void* unk2, uint32_t numverts) override;
	virtual void RenderLine(const Toshi::TVector2& a, const Toshi::TVector2& b) override;
	virtual void RenderLine(float x1, float y1, float x2, float y2) override;
	virtual void RenderOutlineRectangle(const Toshi::TVector2& a, const Toshi::TVector2& b) override;
	virtual void RenderFilledRectangle(const Toshi::TVector2& a, const Toshi::TVector2& b) override;
	virtual void RenderIndexedTriList() override;
	virtual void RenderIndexedTriStripColoursList() override;
	virtual void ScaleCoords(float& x, float& y) override;
	virtual void SetScissors(float f1, float f2, float f3, float f4) override;
	virtual void ClearScissors() override;
	virtual Toshi::T2GUIMaterial* CreateMaterial(const char* texName) override;
	virtual Toshi::T2GUIMaterial* CreateMaterial(Toshi::TTexture* pTex) override;
	virtual void DestroyMaterial(Toshi::T2GUIMaterial* pMat) override;
	virtual uint32_t GetWidth(Toshi::T2GUIMaterial* pMat) override;
	virtual uint32_t GetHeight(Toshi::T2GUIMaterial* pMat) override;

private:
	Toshi::T2GUITransform* m_pTransforms;
	size_t m_iTransformStackPointer;
	uint32_t m_ui32Colour;
	bool m_bIsInScene;
	Toshi::T2GUIMaterial* m_pMaterial;
};