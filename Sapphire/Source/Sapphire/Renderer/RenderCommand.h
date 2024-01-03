#ifndef SPH_RENDERER_COMMAND_H
#define SPH_RENDERER_COMMAND_H

#include "RendererAPI.h"

namespace sph
{
	class RenderCommand
	{
	public:
		inline static void Init() { s_rendererAPI->Init(); }
		inline static void SetViewport(uint32_t _x, uint32_t _y, uint32_t _width, uint32_t _height) { s_rendererAPI->SetViewport(_x, _y, _width, _height); }
		inline static void SetClearColor(const glm::vec4& _color) { s_rendererAPI->SetClearColor(_color); }
		inline static void Clear() { s_rendererAPI->Clear(); }

		inline static void DrawIndexed(const std::shared_ptr<VertexArray>& _vertexArray, uint32_t _indexCount = 0) { s_rendererAPI->DrawIndexed(_vertexArray, _indexCount); }
	
		inline static RendererAPI::API GetAPI() { return s_rendererAPI->GetAPI(); }
	private:
		~RenderCommand();
	private:
		static RendererAPI* s_rendererAPI;
	};
}
#endif