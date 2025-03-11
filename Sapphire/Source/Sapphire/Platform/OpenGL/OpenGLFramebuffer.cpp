#include "sphpch.h"
#include <glad/glad.h>

#include "OpenGLFramebuffer.h"
#include "Sapphire/Renderer/Texture.h"
#include "Sapphire/Core/Log.h"

namespace sph
{
	static constexpr uint32_t MAX_FRAMEBUFFER_SIZE = 8192;

	namespace Utilities
	{
		static GLenum TextureTarget(bool _multisampled)
		{
			return _multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
		}

		static void CreateTextures(bool _multisampled, uint32_t* _outID, uint32_t _count)
		{
			glCreateTextures(TextureTarget(_multisampled), _count, _outID);
		}

		static void BindTexture(bool _multisampled, uint32_t _id)
		{
			glBindTexture(TextureTarget(_multisampled), _id);
		}

		static void AttachColorTexture(uint32_t _id, int _samples, GLenum _internalFormat, GLenum _format, uint32_t _width, uint32_t _height, int _index)
		{
			bool multisampled = _samples > 1;
			if (multisampled)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, _samples, _internalFormat, _width, _height, GL_FALSE);
			}
			else
			{
				glTexImage2D(GL_TEXTURE_2D, 0, _internalFormat, _width, _height, 0, _format, GL_UNSIGNED_BYTE, nullptr);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + _index, TextureTarget(multisampled), _id, 0);
		}

		static void AttachDepthTexture(uint32_t _id, int _samples, GLenum _format, GLenum _attachmentType, uint32_t _width, uint32_t _height)
		{
			bool multisampled = _samples > 1;
			if (multisampled)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, _samples, _format, _width, _height, GL_FALSE);
			}
			else
			{
				glTexStorage2D(GL_TEXTURE_2D, 1, _format, _width, _height);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, _attachmentType, TextureTarget(multisampled), _id, 0);
		}

		static bool IsDepthFormat(FramebufferTextureFormat _format)
		{
			switch (_format)
			{
			case FramebufferTextureFormat::DEPTH24STENCIL8:  return true;
			}

			return false;
		}

		static GLenum FramebufferTextureFormatToGL(FramebufferTextureFormat _format)
		{
			switch (_format)
			{
			case FramebufferTextureFormat::RGBA8:       return GL_RGBA8;
			case FramebufferTextureFormat::RED_INTEGER: return GL_RED_INTEGER;
			}

			ASSERT(false, "Unknown format!");
			return 0;
		}
	}

	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& _specification)
		: m_specification(_specification)
	{
		SPH_PROFILE_FUNCTION();

		for (auto spec : m_specification.Attachments.Attachments)
		{
			if (Utilities::IsDepthFormat(spec.TextureFormat))
			{
				m_depthAttachmentSpecification = spec;
				continue;
			}

			m_colorAttachmentSpecifications.emplace_back(spec);
		}

		Resize();
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		SPH_PROFILE_FUNCTION();
		glDeleteFramebuffers(1, &m_rendererID);
		glDeleteTextures(m_colorAttachments.size(), m_colorAttachments.data());
		glDeleteTextures(1, &m_depthAttachment);
	}

	void OpenGLFramebuffer::Bind()
	{
		SPH_PROFILE_FUNCTION();

		glBindFramebuffer(GL_FRAMEBUFFER, m_rendererID);
		glViewport(0, 0, m_specification.Width, m_specification.Height);
	}

	void OpenGLFramebuffer::Unbind()
	{
		SPH_PROFILE_FUNCTION();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::Resize()
	{
		SPH_PROFILE_FUNCTION();

		// Delete old framebuffer
		if (m_rendererID)
		{
			glDeleteFramebuffers(1, &m_rendererID);
			glDeleteTextures(m_colorAttachments.size(), m_colorAttachments.data());
			glDeleteTextures(1, &m_depthAttachment);

			m_colorAttachments.clear();
			m_depthAttachment = 0;
		}

		glCreateFramebuffers(1, &m_rendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_rendererID);

		bool multisample = m_specification.Samples > 1;

		// Attachments
		if (m_colorAttachmentSpecifications.size())
		{
			m_colorAttachments.resize(m_colorAttachmentSpecifications.size());
			Utilities::CreateTextures(multisample, m_colorAttachments.data(), m_colorAttachments.size());

			for (size_t i = 0; i < m_colorAttachments.size(); i++)
			{
				Utilities::BindTexture(multisample, m_colorAttachments[i]);
				switch (m_colorAttachmentSpecifications[i].TextureFormat)
				{
				case FramebufferTextureFormat::RGBA8:

					Utilities::AttachColorTexture(m_colorAttachments[i], m_specification.Samples, GL_RGBA8, GL_RGBA, m_specification.Width, m_specification.Height, i);
					break;
				case FramebufferTextureFormat::RED_INTEGER:
					Utilities::AttachColorTexture(m_colorAttachments[i], m_specification.Samples, GL_R32I, GL_RED_INTEGER, m_specification.Width, m_specification.Height, i);
					break;
				}
			}
		}

		if (m_depthAttachmentSpecification.TextureFormat != FramebufferTextureFormat::None)
		{
			Utilities::CreateTextures(multisample, &m_depthAttachment, 1);
			Utilities::BindTexture(multisample, m_depthAttachment);
			switch (m_depthAttachmentSpecification.TextureFormat)
			{
			case FramebufferTextureFormat::DEPTH24STENCIL8:
				Utilities::AttachDepthTexture(m_depthAttachment, m_specification.Samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_specification.Width, m_specification.Height);
				break;
			}
		}

		if (m_colorAttachments.size() > 1)
		{
			ASSERT(m_colorAttachments.size() <= 4, "Framebuffer: Only 4 color attachments are supported!");
			GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			glDrawBuffers(m_colorAttachments.size(), buffers);
		}
		else if (m_colorAttachments.empty())
		{
			glDrawBuffer(GL_NONE);
		}
		ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::Resize(uint32_t _width, uint32_t _height)
	{
		SPH_PROFILE_FUNCTION();

		if (m_specification.Width == _width && m_specification.Height == _height) return;

		if (_width <= 0 || _height <= 0 || _width > MAX_FRAMEBUFFER_SIZE || _height > MAX_FRAMEBUFFER_SIZE)
		{
			LogError("Attempted to resize framebuffer to an invalid value: {} - {}!", _width, _height);
			return;
		}
		m_specification.Width = _width;
		m_specification.Height = _height;

		Resize();
	}

	void OpenGLFramebuffer::Clear()
	{
		SPH_PROFILE_FUNCTION();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	int32_t OpenGLFramebuffer::ReadPixel(uint32_t _attachmentIndex, int32_t _x, int32_t _y)
	{
		ASSERT(_attachmentIndex < m_colorAttachments.size(), "Framebuffer: Color attachment index is invalid!");

		int32_t pixelData = -1;

		glReadBuffer(GL_COLOR_ATTACHMENT0 + _attachmentIndex);
		glReadPixels(_x, _y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);

		return pixelData;
	}

	void OpenGLFramebuffer::ClearAttachment(uint32_t _attachmentIndex, int32_t _value)
	{
		ASSERT(_attachmentIndex < m_colorAttachments.size(), "Framebuffer: Color attachment index is invalid!");

		auto& spec = m_colorAttachmentSpecifications[_attachmentIndex];

		glClearTexImage(m_colorAttachments[_attachmentIndex], 0,
			Utilities::FramebufferTextureFormatToGL(spec.TextureFormat),
			GL_INT, &_value);
	}

	uint32_t OpenGLFramebuffer::GetColorAttachmentRendererID(uint32_t _index) const
	{
		ASSERT(_index < m_colorAttachments.size(), "Framebuffer: Color attachment index is invalid!");

		return m_colorAttachments[_index];
	}
}