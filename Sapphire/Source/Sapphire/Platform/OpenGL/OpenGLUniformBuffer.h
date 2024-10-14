#ifndef SPH_OPENGL_UNIFORMBUFFER_H
#define SPH_OPENGL_UNIFORMBUFFER_H

#include <Sapphire/Renderer/UniformBuffer.h>

namespace sph
{
	class OpenGLUniformBuffer : public UniformBuffer
	{
	public:
		OpenGLUniformBuffer(const UniformBufferLayout& _layout, uint32_t _binding);
		virtual ~OpenGLUniformBuffer();

		virtual void SetData(const void* _data, uint32_t _size, uint32_t _offset = 0) override;

		virtual void Bind() const override;
		virtual void Unbind() const override;
	};
}
#endif