#include "sphpch.h"

#include "VertexArray.h"
#include "RendererAPI.h"

#include "Sapphire/Core/Log.h"
#include "Sapphire/Platform/OpenGL/OpenGLVertexArray.h"

namespace sph
{
	sph::Ref<VertexArray> VertexArray::Create()
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:
			ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLVertexArray>();
		}
		ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}