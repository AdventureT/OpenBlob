#pragma once
#include <Toshi/Render/TViewport.h>
#include <Toshi/Utils/TSingleton.h>

class AImGuiRenderer : public Toshi::TSingleton<AImGuiRenderer>
{
public:
	AImGuiRenderer();

	TBOOL CreateTRender();
	TBOOL CreateMainViewport();
	TBOOL CreateImGui();

	void Update(float a_fDeltaTime);

private:
	Toshi::TViewport* m_pViewport;
};
