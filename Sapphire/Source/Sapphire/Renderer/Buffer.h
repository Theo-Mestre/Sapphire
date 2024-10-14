#ifndef SPH_INDEX_BUFFER_H
#define SPH_INDEX_BUFFER_H

#include "Sapphire/Core/Core.h"

namespace sph
{
	enum class ShaderDataType
	{
		None = 0,
		Float, Float2, Float3, Float4,
		Mat3, Mat4,
		Int, Int2, Int3, Int4,
		UInt, UInt2, UInt3, UInt4,
		Bool,
		Custom
	};

	uint32_t ShaderDataTypeSize(ShaderDataType _type);

	struct BufferElement
	{
		std::string name;
		ShaderDataType type;
		uint32_t size;
		uint32_t offset;
		bool normalized;

		BufferElement();
		BufferElement(ShaderDataType _type, const std::string& _name, bool _normalized = false);
		uint32_t GetComponentCount() const;
	};

	class BufferLayout
	{
	public:
		BufferLayout();
		BufferLayout(const std::initializer_list<BufferElement>& _elements);

		inline const std::vector<BufferElement>& GetElements() const { return m_elements; }
		inline uint32_t GetStride() const { return m_stride; }

		inline std::vector<BufferElement>::iterator begin() { return m_elements.begin(); }
		inline std::vector<BufferElement>::iterator end() { return m_elements.end(); }
		inline std::vector<BufferElement>::const_iterator begin() const { return m_elements.begin(); }
		inline std::vector<BufferElement>::const_iterator end() const { return m_elements.end(); }
	private:
		void CalculateOffsetsAndStride();
	private:
		std::vector<BufferElement> m_elements;
		uint32_t m_stride;
	};

	class VertexBuffer
	{
	public:
		VertexBuffer() = default;
		virtual ~VertexBuffer() = default;

		static Ref<VertexBuffer> Create(uint32_t _size);
		static Ref<VertexBuffer> Create(const void* _data, uint32_t _size);

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual void SetData(const void* _data, uint32_t _size) = 0;

		virtual const BufferLayout& GetLayout() const = 0;
		virtual void SetLayout(const BufferLayout& _layout) = 0;
	};

	class IndexBuffer
	{
	public:
		IndexBuffer() = default;
		virtual ~IndexBuffer() = default;

		static Ref<IndexBuffer> Create(const uint32_t* _data, uint16_t _count);

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual uint16_t GetCount() const = 0;
	};
}
#endif	
