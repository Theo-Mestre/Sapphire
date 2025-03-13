#ifndef SPH_FRAMEBUFFER_H
#define SPH_FRAMEBUFFER_H

#include "Sapphire/Core/Core.h"

namespace sph
{
	enum class FramebufferTextureFormat
	{
		None = 0,

		// Color
		RGBA8,
		RED_INTEGER,

		// Depth/stencil
		DEPTH24STENCIL8,

		// Defaults
		Depth = DEPTH24STENCIL8
	};

	struct FramebufferTextureSpecification
	{
		FramebufferTextureSpecification() = default;
		FramebufferTextureSpecification(FramebufferTextureFormat _format)
			: TextureFormat(_format) { }

		FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;
	};

	struct FramebufferAttachmentSpecification
	{
		FramebufferAttachmentSpecification() = default;
		FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> _attachments)
			: Attachments(_attachments) { }

		std::vector<FramebufferTextureSpecification> Attachments;
	};

	struct FramebufferSpecification
	{
		uint32_t Width = 0;
		uint32_t Height = 0;
		FramebufferAttachmentSpecification Attachments;
		uint32_t Samples = 1;
	};

	class Texture2D;

	class Framebuffer
	{
	public:
		Framebuffer() = default;
		virtual ~Framebuffer() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void Resize() = 0;
		virtual void Resize(uint32_t _width, uint32_t _height) = 0;
		virtual void Clear() = 0;

		virtual int32_t ReadPixel(uint32_t _attachmentIndex, int32_t _x, int32_t _y) = 0;

		virtual void ClearAttachment(uint32_t _attachmentIndex, int32_t _value) = 0;
		
		virtual uint32_t GetColorAttachmentRendererID(uint32_t _index = 0) const = 0;
		virtual const FramebufferSpecification& GetSpecification() const = 0;

		static Ref<Framebuffer> Create(const FramebufferSpecification& _spec);

	protected:
		uint32_t m_rendererID = -1;
	};
}
#endif