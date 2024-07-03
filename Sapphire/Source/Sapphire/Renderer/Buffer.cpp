#include "sphpch.h"
#include <glad/glad.h>

#include "Buffer.h"
#include "Renderer.h"
#include "Sapphire/Platform/OpenGL/OpenGLBuffer.h"
#include "Sapphire/Core/Log.h"

namespace sph
{
	uint32_t ShaderDataTypeSize(ShaderDataType _type)
	{
		switch (_type)
		{
		case ShaderDataType::Float:		return 4;
		case ShaderDataType::Float2:	return 4 * 2;
		case ShaderDataType::Float3:	return 4 * 3;
		case ShaderDataType::Float4:	return 4 * 4;
		case ShaderDataType::Mat3:		return 4 * 3 * 3;
		case ShaderDataType::Mat4:		return 4 * 4 * 4;
		case ShaderDataType::Int:		return 4;
		case ShaderDataType::Int2:		return 4 * 2;
		case ShaderDataType::Int3:		return 4 * 3;
		case ShaderDataType::Int4:		return 4 * 4;
		case ShaderDataType::UInt:		return 4;
		case ShaderDataType::UInt2:		return 4 * 2;
		case ShaderDataType::UInt3:		return 4 * 3;
		case ShaderDataType::UInt4:		return 4 * 4;
		case ShaderDataType::Bool:		return 1;
		}

		ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	BufferElement::BufferElement(ShaderDataType _type, const std::string& _name, bool _normalized)
		: name(_name), type(_type), size(ShaderDataTypeSize(_type)), offset(0), normalized(_normalized)
	{
	}

	uint32_t BufferElement::GetComponentCount() const
	{
		switch (type)
		{
		case ShaderDataType::Float:		return 1;
		case ShaderDataType::Float2:	return 2;
		case ShaderDataType::Float3:	return 3;
		case ShaderDataType::Float4:	return 4;
		case ShaderDataType::Mat3:		return 3 * 3;
		case ShaderDataType::Mat4:		return 4 * 4;
		case ShaderDataType::Int:		return 1;
		case ShaderDataType::Int2:		return 2;
		case ShaderDataType::Int3:		return 3;
		case ShaderDataType::Int4:		return 4;
		case ShaderDataType::UInt:		return 1;
		case ShaderDataType::UInt2:		return 2;
		case ShaderDataType::UInt3:		return 3;
		case ShaderDataType::UInt4:		return 4;
		case ShaderDataType::Bool:		return 1;
		}

		ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	BufferLayout::BufferLayout(const std::initializer_list<BufferElement>& _elements)
		: m_elements(_elements)
	{
		CalculateOffsetsAndStride();
	}

	void BufferLayout::CalculateOffsetsAndStride()
	{
		m_stride = 0;
		for (auto& element : m_elements)
		{
			element.offset = m_stride;
			m_stride += element.size;
		}
	}

	Ref<VertexBuffer> VertexBuffer::Create(const void* _data, uint32_t _size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLVertexBuffer>((float*)_data, _size);
		}
		ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(const uint32_t* _data, uint16_t _count)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLIndexBuffer>(_data, _count);
		}
		ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}