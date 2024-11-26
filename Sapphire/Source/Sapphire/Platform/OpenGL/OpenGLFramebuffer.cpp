#include "sphpch.h"
#include <glad/glad.h>

#include "OpenGLFramebuffer.h"
#include "Sapphire/Renderer/Texture.h"
#include "Sapphire/Core/Log.h"

namespace sph
{
	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& _specification)
		: m_specification(_specification)
		, m_textureAttachment(nullptr)
	{
		Resize();
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		glDeleteFramebuffers(1, &m_rendererID);
	}

	void OpenGLFramebuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_rendererID);
	}

	void OpenGLFramebuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::AttachTexture()
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textureAttachment->GetRendererID(), 0);
	}

	void OpenGLFramebuffer::Resize()
	{
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
		m_specification.Width = _width;
		m_specification.Height = _height;

		Resize();
	}

	void OpenGLFramebuffer::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
}