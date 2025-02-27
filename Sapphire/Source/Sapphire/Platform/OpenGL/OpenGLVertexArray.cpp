#include "sphpch.h"
#include <glad/glad.h>

#include "OpenGLVertexArray.h"
#include "Sapphire/Renderer/Buffer.h"
#include "Sapphire/Core/Log.h"

namespace sph
{
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType _type)
	{
		SPH_PROFILE_FUNCTION();

		switch (_type)
		{
		case ShaderDataType::Float:		return GL_FLOAT;
		case ShaderDataType::Float2:	return GL_FLOAT;
		case ShaderDataType::Float3:	return GL_FLOAT;
		case ShaderDataType::Float4:	return GL_FLOAT;
		case ShaderDataType::Mat3:		return GL_FLOAT;
		case ShaderDataType::Mat4:		return GL_FLOAT;
		case ShaderDataType::Int:		return GL_INT;
		case ShaderDataType::Int2:		return GL_INT;
		case ShaderDataType::Int3:		return GL_INT;
		case ShaderDataType::Int4:		return GL_INT;
		case ShaderDataType::UInt:		return GL_UNSIGNED_INT;
		case ShaderDataType::UInt2:		return GL_UNSIGNED_INT;
		case ShaderDataType::UInt3:		return GL_UNSIGNED_INT;
		case ShaderDataType::UInt4:		return GL_UNSIGNED_INT;
		case ShaderDataType::Bool:		return GL_BOOL;
		}

		ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
		: m_rendererID(0)
	{
		SPH_PROFILE_FUNCTION();

		glCreateVertexArrays(1, &m_rendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		SPH_PROFILE_FUNCTION();

		glDeleteVertexArrays(1, &m_rendererID);
	}

	void OpenGLVertexArray::Bind() const
	{
		SPH_PROFILE_FUNCTION();

		glBindVertexArray(m_rendererID);

		for (const auto& vertexBuffer : m_vertexBuffers)
		{
			vertexBuffer->Bind();
		}

		m_indexBuffer->Bind();
	}

	void OpenGLVertexArray::Unbind() const
	{
		SPH_PROFILE_FUNCTION();

		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& _vertexBuffer)
	{
		SPH_PROFILE_FUNCTION();

		ASSERT(_vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");

		glBindVertexArray(m_rendererID);
		_vertexBuffer->Bind();

		const auto& layout = _vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(m_vertexBufferIndex);
			glVertexAttribPointer(m_vertexBufferIndex,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.type),
				element.normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				reinterpret_cast<const void*>(static_cast<uintptr_t>(element.offset)));
			m_vertexBufferIndex++;
		}

		m_vertexBuffers.push_back(_vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& _indexBuffer)
	{
		SPH_PROFILE_FUNCTION();

		glBindVertexArray(m_rendererID);
		_indexBuffer->Bind();

		m_indexBuffer = _indexBuffer;
	}
}