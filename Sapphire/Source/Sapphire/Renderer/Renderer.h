#ifndef SPH_RENDERER_H
#define SPH_RENDERER_H

#include "RenderCommand.h"

namespace sph
{
	class Shader;
	class Material;
	class VertexArray;
	class OrthographicCamera;

	class Renderer
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& _camera);
		static void EndScene();

		static void Submit(const std::shared_ptr<VertexArray>& _vertexArray, const std::shared_ptr<Material>& _material, const glm::mat4& _tranform = glm::mat4(1));

		static void OnWindowResize(uint32_t _width, uint32_t _height);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData s_sceneData;
	};
}
#endif