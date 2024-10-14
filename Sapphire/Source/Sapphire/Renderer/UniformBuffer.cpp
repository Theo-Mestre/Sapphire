#include "sphpch.h"
#include "UniformBuffer.h"

#include "Sapphire/Core/Log.h"
#include "RendererAPI.h"
#include "Sapphire/Platform/OpenGL/OpenGLUniformBuffer.h"

namespace sph
{
	UniformBufferLayout::UniformBufferLayout(const std::initializer_list<BufferElement>& _elements)
		: m_elements(_elements)
	{
		CalculateOffsetsAndStride();
	}

	void UniformBufferLayout::AddElement(ShaderDataType _type, const std::string& _name, bool _normalized)
	{
		m_elements.push_back({ _type, _name, _normalized });
		CalculateOffsetsAndStride();
	}

	void UniformBufferLayout::AddCustomElement(const BufferElement& _element)
	{
		m_elements.push_back(_element);
		CalculateOffsetsAndStride();
	}

	void UniformBufferLayout::CalculateOffsetsAndStride()
	{
		m_stride = 0;
		for (auto& element : m_elements)
		{
			element.offset = m_stride;
			m_stride += element.size;
		}
	}

	Ref<UniformBuffer> UniformBuffer::Create(const UniformBufferLayout& _layout, uint32_t _binding)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:
			ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLUniformBuffer>(_layout, _binding);
		}
		ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}