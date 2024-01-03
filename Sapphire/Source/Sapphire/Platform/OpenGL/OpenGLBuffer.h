#ifndef OPENGL_BUFFER_H
#define OPENGL_BUFFER_H

#include "Sapphire/Renderer/Buffer.h"	

namespace sph
{
	class OpenGLVertexBuffer
		: public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(float* _vertices, uint32_t _size);
		virtual ~OpenGLVertexBuffer();

		virtual inline const BufferLayout& GetLayout() const override { return m_layout; }
		virtual inline void SetLayout(const BufferLayout& _layout) override { m_layout = _layout; }

		virtual void Bind() const override;	
		virtual void Unbind() const override;
		virtual void SetData(const void* _data, uint32_t _size) override;

	private:
		uint32_t m_rendererID;
		BufferLayout m_layout;
	};

	class OpenGLIndexBuffer
		: public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(const uint32_t* _indices, uint16_t _count);
		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual inline uint16_t GetCount() const override { return m_count; }

	private:
		uint16_t m_count;
		uint32_t m_rendererID;
	};
}

#endif