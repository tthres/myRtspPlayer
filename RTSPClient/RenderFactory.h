#pragma once

#include <memory>
#include "IRender.h"

class RenderFactory
{
public:
	static std::unique_ptr<IRender> CreateRender(RenderType type);

};

