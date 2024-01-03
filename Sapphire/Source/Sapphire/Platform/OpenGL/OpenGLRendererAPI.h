#ifndef OPENGL_RENDERER_API_H
#define OPENGL_RENDERER_API_H

#include "Sapphire/Renderer/RendererAPI.h"

namespace sph
{
	class OpenGLRendererAPI 
		: public RendererAPI
	{
	public:
		virtual void Init() override;
		virtual void SetViewport(uint32_t _x, uint32_t _y, uint32_t _width, uint32_t _height) override;
		virtual void SetClearColor(const glm::vec4& _color) override;
		virtual void Clear() override;

		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& _vertexArray, uint32_t _indexCount = 0) override;
	};
}

#endif