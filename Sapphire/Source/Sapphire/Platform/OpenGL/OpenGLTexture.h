#ifndef OPENGL_TEXTURE_H
#define OPENGL_TEXTURE_H

#include "Sapphire/Renderer/Texture.h"

namespace sph
{
	class OpenGLTexture2D
		: public Texture2D
	{
	public:
		OpenGLTexture2D(const std::string& _path, const Properties& _properties = Properties());
		OpenGLTexture2D(uint32_t _width, uint32_t _height, const Properties& _properties = Properties());
		virtual ~OpenGLTexture2D();

		virtual void Bind(uint32_t _slot = 0) const override;
		virtual void Unbind() const override;

		virtual uint32_t GetWidth() const override { return m_width; }
		virtual uint32_t GetHeight() const override { return m_height; }
		virtual glm::vec2 GetSize() const override { return { (float)m_width, (float)m_height }; }

		virtual void SetSize(uint32_t _width, uint32_t _height) override;

		virtual uint32_t GetRendererID() const override { return m_rendererID; }

		virtual void SetData(void* _data, uint32_t _size) override;

		virtual bool operator==(const Texture& _other) const override
		{
			return m_rendererID == ((OpenGLTexture2D&)_other).m_rendererID;
		}
	private:
		void InitProperties(const Properties& _properties);
	private:
		std::string m_path;
		uint32_t m_width;
		uint32_t m_height;
		uint32_t m_rendererID;
		uint32_t m_internalFormat;
		uint32_t m_dataFormat;
	};
}
#endif