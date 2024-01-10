#include "sphpch.h"

#include <glad/glad.h>
#include <stb_image.h>

#include "OpenGLTexture.h"
#include "Sapphire/Log.h"

#include <fstream>

namespace sph
{
	OpenGLTexture2D::OpenGLTexture2D(const std::string& _path)
		: m_path(_path)
	{
		int width, height, channels;
		
		stbi_set_flip_vertically_on_load(1);
		auto data = stbi_load(_path.c_str(), &width, &height, &channels, 0);

		ASSERT(data, "Failed to load image!");

		m_width = width;
		m_height = height;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererID);
		glTextureStorage2D(m_rendererID, 1, GL_RGB8, m_width, m_height);

		glTextureParameterf(m_rendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameterf(m_rendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureSubImage2D(m_rendererID, 0, 0, 0, m_width, m_height, GL_RGB, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	//OpenGLTexture2D::OpenGLTexture2D(uint32_t _width, uint32_t _height)
	//{
	//}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_rendererID);
	}

	void OpenGLTexture2D::Bind(uint32_t _slot) const
	{
		glBindTextureUnit(_slot, m_rendererID);
	}

	void OpenGLTexture2D::Unbind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}