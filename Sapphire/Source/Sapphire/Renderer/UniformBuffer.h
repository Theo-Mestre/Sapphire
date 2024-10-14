#ifndef SPH_UNIFORMBUFFER_H
#define SPH_UNIFORMBUFFER_H

#include "Sapphire/Core/Core.h"
#include "Buffer.h"

namespace sph
{
	class UniformBufferLayout
	{
	public:
		UniformBufferLayout() = default;
		UniformBufferLayout(const std::initializer_list<BufferElement>& _elements);

		inline const std::vector<BufferElement>& GetElements() const { return m_elements; }
		inline uint32_t GetStride() const { return m_stride; }

		void AddElement(ShaderDataType _type, const std::string& _name, bool _normalized = false);
		void AddCustomElement(const BufferElement& _element);

		inline std::vector<BufferElement>::iterator begin() { return m_elements.begin(); }
		inline std::vector<BufferElement>::iterator end() { return m_elements.end(); }
		inline std::vector<BufferElement>::const_iterator begin() const { return m_elements.begin(); }
		inline std::vector<BufferElement>::const_iterator end() const { return m_elements.end(); }
	private:
		void CalculateOffsetsAndStride();
	private:
		std::vector<BufferElement> m_elements;
		uint32_t m_stride = 0;
	};

	class UniformBuffer
	{
	public:
		UniformBuffer() = default;
		virtual ~UniformBuffer() = default;

		virtual void SetData(const void* _data, uint32_t _size, uint32_t _offset = 0) = 0;

		virtual const UniformBufferLayout& GetLayout() const { return m_layout; }
		virtual uint32_t GetBinding() const { return m_binding; }
		virtual uint32_t GetRendererID() const { return m_rendererID; }

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetBindingPoint(uint32_t _binding) { m_binding = _binding; }
		virtual uint32_t GetBindingPoint() const { return m_binding; }

		static Ref<UniformBuffer> Create(const UniformBufferLayout& _layout, uint32_t _binding);
	protected:
		UniformBufferLayout m_layout;
		uint32_t m_rendererID;
		uint32_t m_binding;
	};
}

#endif