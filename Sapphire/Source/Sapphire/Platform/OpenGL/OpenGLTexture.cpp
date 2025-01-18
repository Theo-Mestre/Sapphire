#include "sphpch.h"

#include <glad/glad.h>
#include <stb_image.h>

#include "OpenGLTexture.h"
#include "Sapphire/Core/Log.h"

#include <fstream>

namespace sph
{
	OpenGLTexture2D::OpenGLTexture2D(const std::string& _path, const Properties& _properties)
		: m_path(_path)
	{
		SPH_PROFILE_FUNCTION();

		int channels;
		stbi_set_flip_vertically_on_load(1);
		auto data = stbi_load(_path.c_str(), (int*)&m_width, (int*)&m_height, &channels, 0);

		ASSERT(data, "Failed to load image : \"" + _path + "\"!");

		if (channels == 4)
		{
			m_internalFormat = GL_RGBA8;
			m_dataFormat = GL_RGBA;
		}
		else if (channels == 3)
		{
			m_internalFormat = GL_RGB8;
			m_dataFormat = GL_RGB;
		}

		ASSERT(m_internalFormat & m_dataFormat, "Format not supported!");

		glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererID);
		glTextureStorage2D(m_rendererID, 1, m_internalFormat, m_width, m_height);

		InitProperties(_properties);

		glTextureSubImage2D(m_rendererID, 0, 0, 0, m_width, m_height, m_dataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	OpenGLTexture2D::OpenGLTexture2D(uint32_t _width, uint32_t _height, const Properties& _properties)
		: m_width(_width)
		, m_height(_height)
		, m_internalFormat(GL_RGBA8)
		, m_dataFormat(GL_RGBA)
		, m_rendererID(0)
	{
		SPH_PROFILE_FUNCTION();

		glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererID);
		glTextureStorage2D(m_rendererID, 1, m_internalFormat, m_width, m_height);

		InitProperties(_properties);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		SPH_PROFILE_FUNCTION();

		glDeleteTextures(1, &m_rendererID);
	}

	void OpenGLTexture2D::Bind(uint32_t _slot) const
	{
		SPH_PROFILE_FUNCTION();

		glBindTextureUnit(_slot, m_rendererID);
	}

	void OpenGLTexture2D::Unbind() const
	{
		SPH_PROFILE_FUNCTION();

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void OpenGLTexture2D::SetSize(uint32_t _width, uint32_t _height)
	{
		SPH_PROFILE_FUNCTION();

		m_width = _width;
		m_height = _height;
		glTextureStorage2D(m_rendererID, 1, m_internalFormat, m_width, m_height);
	}

	void OpenGLTexture2D::SetData(void* _data, uint32_t _size)
	{
		SPH_PROFILE_FUNCTION();

		uint32_t bpp = m_dataFormat == GL_RGBA ? 4 : 3;
		ASSERT(_size == m_width * m_height * bpp, "Data must be entire texture!");
		glTextureSubImage2D(m_rendererID, 0, 0, 0, m_width, m_height, m_dataFormat, GL_UNSIGNED_BYTE, _data);
	}

	void OpenGLTexture2D::InitProperties(const Properties& _properties)
	{
		SPH_PROFILE_FUNCTION();

		glTextureParameterf(m_rendererID, GL_TEXTURE_MIN_FILTER, (GLfloat)_properties.MinificationFilter);
		glTextureParameterf(m_rendererID, GL_TEXTURE_MAG_FILTER, (GLfloat)_properties.MagnificationFilter);

		glTextureParameterf(m_rendererID, GL_TEXTURE_WRAP_S, (GLfloat)_properties.WrapModeS);
		glTextureParameterf(m_rendererID, GL_TEXTURE_WRAP_T, (GLfloat)_properties.WrapModeT);
	}
}