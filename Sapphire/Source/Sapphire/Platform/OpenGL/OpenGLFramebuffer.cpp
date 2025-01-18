#include "sphpch.h"
#include <glad/glad.h>

#include "OpenGLFramebuffer.h"
#include "Sapphire/Renderer/Texture.h"
#include "Sapphire/Core/Log.h"

namespace sph
{
	static constexpr uint32_t MAX_FRAMEBUFFER_SIZE = 8192;

	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& _specification)
		: m_specification(_specification)
		, m_textureAttachment(nullptr)
	{
		SPH_PROFILE_FUNCTION();

		Resize();
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		SPH_PROFILE_FUNCTION();

		glDeleteFramebuffers(1, &m_rendererID);
	}

	void OpenGLFramebuffer::Bind()
	{
		SPH_PROFILE_FUNCTION();

		glBindFramebuffer(GL_FRAMEBUFFER, m_rendererID);
	}

	void OpenGLFramebuffer::Unbind()
	{
		SPH_PROFILE_FUNCTION();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::AttachTexture()
	{
		SPH_PROFILE_FUNCTION();

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textureAttachment->GetRendererID(), 0);
	}

	void OpenGLFramebuffer::Resize()
	{
		SPH_PROFILE_FUNCTION();

		// Delete old framebuffer
		if (m_rendererID)
		{
			glDeleteFramebuffers(1, &m_rendererID);
		}

		glCreateFramebuffers(1, &m_rendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_rendererID);

		// Texture Attachment
		if (m_textureAttachment == nullptr)
		{
			m_textureAttachment = Texture2D::Create(m_specification.Width, m_specification.Height);
		}
		m_textureAttachment->Bind();

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textureAttachment->GetRendererID(), 0);

		// Check if the framebuffer is complete
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
}