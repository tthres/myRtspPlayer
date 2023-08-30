#include "RenderFactory.h"
#include "D3DRender.h"

std::unique_ptr<IRender> RenderFactory::CreateRender(RenderType type)
{
	if (type == RenderType::D3D) {
		return std::make_unique<D3DRender>();
	}
	return nullptr;
}
