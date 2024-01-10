#include "sphpch.h"
#include <glad/glad.h>

#include "Shader.h"
#include "Sapphire/Log.h"
#include "Sapphire/Renderer/Renderer.h"

#include "Sapphire/Platform/OpenGL/OpenGLShader.h"

namespace sph
{
	Shader* Shader::Create(const std::string& _vertexSource, const std::string& _fragmentSource)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return new OpenGLShader(_vertexSource, _fragmentSource);
		}

		ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}