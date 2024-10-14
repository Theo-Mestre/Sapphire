#include "sphpch.h"
#include <glad/glad.h>

#include "OpenGLUniformBuffer.h"

namespace sph
{
	OpenGLUniformBuffer::OpenGLUniformBuffer(const UniformBufferLayout& _layout, uint32_t _binding)
	{
		m_layout = _layout;
		m_binding = _binding;

		glCreateBuffers(1, &m_rendererID);
		glBindBuffer(GL_UNIFORM_BUFFER, m_rendererID);
		glBufferData(GL_UNIFORM_BUFFER, m_layout.GetStride(), nullptr, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, m_binding, m_rendererID);
	}

	OpenGLUniformBuffer::~OpenGLUniformBuffer()
	{
		glDeleteBuffers(1, &m_rendererID);
	}

	void OpenGLUniformBuffer::SetData(const void* _data, uint32_t _size, uint32_t _offset)
	{
		glBindBuffer(GL_UNIFORM_BUFFER, m_rendererID);
		glBufferSubData(GL_UNIFORM_BUFFER, _offset, _size, _data);
	}

	void OpenGLUniformBuffer::Bind() const
	{
		glBindBuffer(GL_UNIFORM_BUFFER, m_rendererID);
	}

	void OpenGLUniformBuffer::Unbind() const
	{
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	void OpenGLUniformBuffer::SetBindingPoint(uint32_t _binding)
	{
		m_binding = _binding; 
		glBindBufferBase(GL_UNIFORM_BUFFER, m_binding, m_rendererID);
	}
}