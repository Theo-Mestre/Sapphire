#ifndef SPH_OPENGL_FRAMEBUFFER_H
#define SPH_OPENGL_FRAMEBUFFER_H

#include "Sapphire/Renderer/Framebuffer.h"

namespace sph
{
	class Texture2D;

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

		virtual const Ref<Texture2D>& GetTextureAttachment() const override { return m_textureAttachment; }
		virtual const FramebufferSpecification& GetSpecification() const override { return m_specification; }

	private:
		FramebufferSpecification m_specification;
		Ref<Texture2D> m_textureAttachment;
	};
}

#endif