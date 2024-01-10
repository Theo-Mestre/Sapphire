#include "sphpch.h"
#include "Texture.h"
#include "Renderer.h"
#include "Sapphire/Log.h"
#include "Sapphire/Platform/OpenGL/OpenGLTexture.h"

namespace sph
{
	Ref<Texture2D> sph::Texture2D::Create(const std::string& _path)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLTexture2D>(_path);
		default:
			ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		}
	}

	Ref<Texture2D> sph::Texture2D::Create(uint32_t _width, uint32_t _height)
	{
		return Ref<Texture2D>();
	}
}
