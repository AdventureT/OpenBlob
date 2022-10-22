#pragma once
#include "AExampleClass.h"

#include <Toshi.h>
#include <Toshi/Sound/TSound.h>
#include <Toshi/Render/TRender.h>

class AApplication : public Toshi::TApplication
{
public:
	AApplication() = default;
	~AApplication() { }

	virtual bool OnCreate() override;
	virtual bool OnUpdate() override;

};