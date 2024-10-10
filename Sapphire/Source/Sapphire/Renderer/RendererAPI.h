#ifndef SPH_RENDERERAPI_H
#define SPH_RENDERERAPI_H

namespace sph
{
	class VertexArray;

	class RendererAPI
	{
	public:
		enum class API
		{
			None = 0,
			OpenGL = 1
		};
	public:
		virtual ~RendererAPI() = default;

		virtual void Init() = 0;
		virtual void SetViewport(uint32_t _x, uint32_t _y, uint32_t _width, uint32_t _height) = 0;
		virtual void SetClearColor(const glm::vec4& _color) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& _vertexArray, uint32_t _indexCount = 0) = 0;

		inline static const API GetAPI() { return s_api; }
	private:
		static API s_api;
	};
}
#endif