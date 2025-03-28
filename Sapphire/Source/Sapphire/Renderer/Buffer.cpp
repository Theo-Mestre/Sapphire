#include "sphpch.h"
#include <glad/glad.h>

#include "Buffer.h"
#include "Sapphire/Platform/OpenGL/OpenGLBuffer.h"
#include "Sapphire/Core/Log.h"
#include "Sapphire/Renderer/RendererAPI.h"

namespace sph
{
	uint32_t ShaderDataTypeSize(ShaderDataType _type)
	{
		SPH_PROFILE_FUNCTION();

		switch (_type)
		{
		case ShaderDataType::Float:		return 4;
		case ShaderDataType::Float2:	return 4 * 2;
		case ShaderDataType::Float3:	return 4 * 3;
		case ShaderDataType::Float4:	return 4 * 4;
		case ShaderDataType::Mat3:		return 3 * 3 * 4;
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
		case ShaderDataType::Custom:	return 0;
		}

		ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}


	BufferElement::BufferElement()
		: Name("")
		, Type(ShaderDataType::None)
		, Size(0)
		, Offset(0)
		, Normalized(false)
	{
		SPH_PROFILE_FUNCTION();
	}

	BufferElement::BufferElement(ShaderDataType _type, const std::string& _name, bool _normalized)
		: Name(_name)
		, Type(_type)
		, Size(ShaderDataTypeSize(_type))
		, Offset(0)
		, Normalized(_normalized)
	{
		SPH_PROFILE_FUNCTION();
	}

	uint32_t BufferElement::GetComponentCount() const
	{
		SPH_PROFILE_FUNCTION();

		switch (Type)
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

	BufferLayout::BufferLayout()
		: m_elements()
		, m_stride(0)
	{
		SPH_PROFILE_FUNCTION();

	}

	BufferLayout::BufferLayout(std::initializer_list<BufferElement> _elements)
		: m_elements(_elements)
		, m_stride(0)
	{
		SPH_PROFILE_FUNCTION();

		CalculateOffsetsAndStride();
	}

	void BufferLayout::CalculateOffsetsAndStride()
	{
		SPH_PROFILE_FUNCTION();

		m_stride = 0;
		for (auto& element : m_elements)
		{
			element.Offset = m_stride;
			m_stride += element.Size;
		}
	}

	Ref<VertexBuffer> VertexBuffer::Create(uint32_t _size)
	{
		SPH_PROFILE_FUNCTION();

		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:
			ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLVertexBuffer>(_size);
		}
		ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::Create(const void* _data, uint32_t _size)
	{
		SPH_PROFILE_FUNCTION();

		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:
			ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLVertexBuffer>((float*)_data, _size);
		}
		ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(const uint32_t* _data, uint16_t _count)
	{
		SPH_PROFILE_FUNCTION();

		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:
			ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLIndexBuffer>(_data, _count);
		}
		ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}