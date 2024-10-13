#include "sphpch.h"
#include "Texture.h"
#include "Sapphire/Core/Log.h"
#include "RendererAPI.h"
#include "Sapphire/Platform/OpenGL/OpenGLTexture.h"

namespace sph
{
	Ref<Texture2D> sph::Texture2D::Create(const std::string& _path, const Properties& _properties)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:
			ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLTexture2D>(_path, _properties);
		default:
			ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		}
	}

	Ref<Texture2D> sph::Texture2D::Create(uint32_t _width, uint32_t _height, const Properties& _properties)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:
			ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLTexture2D>(_width, _height, _properties);
		default:
			ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		}
	}
}
