#include "sphpch.h"
#include "Renderer2D.h"

#include "Sapphire/Renderer//RenderCommand.h"

namespace sph
{
	void Renderer2D::OnWindowResize(uint32_t _width, uint32_t _height)
	{
		RenderCommand::SetViewport(0, 0, _width, _height);
	}
}