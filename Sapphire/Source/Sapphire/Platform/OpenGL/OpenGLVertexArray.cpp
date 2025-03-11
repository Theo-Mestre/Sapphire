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
			if (element.Type <= ShaderDataType::Float4)
			{
				glEnableVertexAttribArray(m_vertexBufferIndex);
				glVertexAttribPointer(m_vertexBufferIndex,
					element.GetComponentCount(),
					ShaderDataTypeToOpenGLBaseType(element.Type),
					element.Normalized ? GL_TRUE : GL_FALSE,
					layout.GetStride(),
					(const void*)element.Offset);
				m_vertexBufferIndex++;
				continue;
			}
			else if (element.Type <= ShaderDataType::Mat4)
			{
				uint8_t count = element.GetComponentCount();
				for (uint8_t i = 0; i < count; i++)
				{
					glEnableVertexAttribArray(m_vertexBufferIndex);
					glVertexAttribPointer(m_vertexBufferIndex,
						count,
						ShaderDataTypeToOpenGLBaseType(element.Type),
						element.Normalized ? GL_TRUE : GL_FALSE,
						layout.GetStride(),
						(const void*)(element.Offset + sizeof(float) * count * i));
					glVertexAttribDivisor(m_vertexBufferIndex, 1);
					m_vertexBufferIndex++;
				}
				continue;
			}
			else if (element.Type <= ShaderDataType::Bool)
			{
				glEnableVertexAttribArray(m_vertexBufferIndex);
				glVertexAttribIPointer(m_vertexBufferIndex,
					element.GetComponentCount(),
					ShaderDataTypeToOpenGLBaseType(element.Type),

					layout.GetStride(),
					(const void*)element.Offset);
				m_vertexBufferIndex++;
				break;
				continue;
			}
			else
			{
				ASSERT(false, "Unknown ShaderDataType!");
			}
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