#include "sphpch.h"

#include "Framebuffer.h"
#include "RendererAPI.h"
#include "Sapphire/Core/Log.h"
#include "Sapphire/Platform/OpenGL/OpenGLFramebuffer.h"

namespace sph
{
	Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& _spec)
	{
		SPH_PROFILE_FUNCTION();

		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:
			ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLFramebuffer>(_spec);
		default:
			break;
		}
		ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}