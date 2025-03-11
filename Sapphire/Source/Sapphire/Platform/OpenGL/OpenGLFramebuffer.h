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

		virtual void Resize() override;
		virtual void Resize(uint32_t _width, uint32_t _height) override;
		virtual void Clear() override;

		virtual int32_t ReadPixel(uint32_t _attachmentIndex, int32_t _x, int32_t _y) override;

		virtual uint32_t GetColorAttachmentRendererID(uint32_t _index = 0) const override;
		virtual const FramebufferSpecification& GetSpecification() const override { return m_specification; }

	private:
		FramebufferSpecification m_specification;

		std::vector<FramebufferTextureSpecification> m_colorAttachmentSpecifications;
		FramebufferTextureSpecification m_depthAttachmentSpecification = FramebufferTextureFormat::None;

		std::vector<uint32_t> m_colorAttachments;
		uint32_t m_depthAttachment = 0;
	};
}

#endif