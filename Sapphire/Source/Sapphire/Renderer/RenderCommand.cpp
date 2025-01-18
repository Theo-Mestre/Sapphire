#include "sphpch.h"
#include "RenderCommand.h"

#include "Sapphire/Platform/OpenGL/OpenGLRendererAPI.h"

namespace sph
{
	RendererAPI* RenderCommand::s_rendererAPI = new OpenGLRendererAPI;

	RenderCommand::~RenderCommand()
	{
		SPH_PROFILE_FUNCTION();

		delete s_rendererAPI;
	}
}
