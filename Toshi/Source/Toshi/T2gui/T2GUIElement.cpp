#include "ToshiPCH.h"
#include "T2GUIElement.h"

void Toshi::T2GUIElement::Tick(float a_tickrate)
{
}

void Toshi::T2GUIElement::SkipRender()
{
}

void Toshi::T2GUIElement::PreRender()
{
}

void Toshi::T2GUIElement::Render()
{
}

bool Toshi::T2GUIElement::IsPointInside(const TVector2& a_rVector2)
{
	float a_minX;
	float a_minY;
	float a_maxX;
	float a_maxY;

	GetMins(a_minX, a_minY);
	GetMaxs(a_maxX, a_maxY);

	return a_minX <= a_rVector2.x && a_rVector2.x <= a_maxX && a_minY <= a_rVector2.y && a_rVector2.y < a_maxY ? true : false;
}

void Toshi::T2GUIElement::PostRender()
{
}

void Toshi::T2GUIElement::GetMins(float& a_minX, float& a_minY)
{
	float width;
	float height;

	GetDimensions(width, height);
	a_minX = -0.5f * width;
	a_minY = -0.5f * height;
}

void Toshi::T2GUIElement::GetMaxs(float& a_maxX, float& a_maxY)
{
	float width;
	float height;

	GetDimensions(width, height);
	a_maxX = 0.5f * width;
	a_maxY = 0.5f * height;
}
