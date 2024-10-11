#include "sphpch.h"
#include "Renderer.h"

#include "Sapphire/Renderer//RenderCommand.h"

namespace sph
{
	void Renderer::OnWindowResize(uint32_t _width, uint32_t _height)
	{
		RenderCommand::SetViewport(0, 0, _width, _height);
	}
}