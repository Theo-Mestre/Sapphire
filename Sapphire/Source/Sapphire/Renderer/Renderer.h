#ifndef SPH_RENDERER_H
#define SPH_RENDERER_H

#include "RenderCommand.h"

namespace sph
{
	class Shader;

	class Renderer
	{
	public:
		static void Init();
		static void OnWindowResize(uint32_t _width, uint32_t _height);

		static void BeginScene();
		static void EndScene();

		static void Submit(const std::shared_ptr<VertexArray>& _vertexArray, const std::shared_ptr<Shader>& _shader);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* s_sceneData;
	};
}
#endif