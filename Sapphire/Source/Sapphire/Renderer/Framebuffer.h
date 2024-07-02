#ifndef SPH_FRAMEBUFFER_H
#define SPH_FRAMEBUFFER_H

#include "Sapphire/Core/Core.h"

namespace sph
{
	struct FramebufferSpecification
	{
		uint32_t Width, Height;
		uint32_t Samples = 1;

		/*
			filtering: linear, nearest
		*/
	};

	class Framebuffer
	{
	public:
		Framebuffer() = default;
		virtual ~Framebuffer() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void AttachTexture() = 0;
		virtual void Resize() = 0;
		virtual void Resize(uint32_t _width, uint32_t _height) = 0;
		virtual void Clear() = 0;

		virtual uint32_t GetColorAttachmentRendererID() const = 0;
		virtual const FramebufferSpecification& GetSpecification() const = 0;

		static Ref<Framebuffer> Create(const FramebufferSpecification& spec);

	private:
		uint32_t m_RendererID;
	};
}
#endif