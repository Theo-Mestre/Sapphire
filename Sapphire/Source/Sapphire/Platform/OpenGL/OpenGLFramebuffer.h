#ifndef SPH_OPENGL_FRAMEBUFFER_H
#define SPH_OPENGL_FRAMEBUFFER_H

#include "Sapphire/Renderer/Framebuffer.h"

namespace sph
{
	class OpenGLFramebuffer 
		: public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpecification& _specification);
		virtual ~OpenGLFramebuffer();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void AttachTexture() override;
		virtual void Resize() override;
		virtual void Resize(uint32_t _width, uint32_t _height) override;
		virtual void Clear() override;

		virtual uint32_t GetColorAttachmentRendererID() const override { return m_colorAttachment; }
		virtual const FramebufferSpecification& GetSpecification() const override { return m_specification; }

	private:
		uint32_t m_rendererID;
		uint32_t m_colorAttachment;
		uint32_t m_depthAttachment;
		FramebufferSpecification m_specification;
	};
}

#endif