#include "sphpch.h"

#include "VertexArray.h"
#include "Renderer.h"

#include "Sapphire/Log.h"
#include "Sapphire/Platform/OpenGL/OpenGLVertexArray.h"

namespace sph
{
	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:
			return new OpenGLVertexArray();
		}
		ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}