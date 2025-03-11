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
		SPH_PROFILE_FUNCTION();

		CalculateOffsetsAndStride();
	}

	void UniformBufferLayout::AddElement(ShaderDataType _type, const std::string& _name, bool _normalized)
	{
		SPH_PROFILE_FUNCTION();

		m_elements.push_back({ _type, _name, _normalized });
		CalculateOffsetsAndStride();
	}

	void UniformBufferLayout::AddCustomElement(const BufferElement& _element)
	{
		SPH_PROFILE_FUNCTION();

		m_elements.push_back(_element);
		CalculateOffsetsAndStride();
	}

	void UniformBufferLayout::CalculateOffsetsAndStride()
	{
		SPH_PROFILE_FUNCTION();

		m_stride = 0;
		for (auto& element : m_elements)
		{
			element.Offset = m_stride;
			m_stride += element.Size;
		}
	}

	Ref<UniformBuffer> UniformBuffer::Create(const UniformBufferLayout& _layout, uint32_t _binding)
	{
		SPH_PROFILE_FUNCTION();

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