#include "sphpch.h"
#include "Renderer.h"

#include "Sapphire/Renderer/Shader.h"
#include "Sapphire/Renderer/Camera.h"
#include "Sapphire/Renderer/Material.h"
#include "Sapphire/Renderer/VertexArray.h"
#include "Sapphire/Renderer/Renderer2D.h"
#include "Sapphire/Platform/OpenGL/OpenGLShader.h"

namespace sph
{
	Renderer::SceneData Renderer::s_sceneData = Renderer::SceneData();

	void Renderer::Init()
	{
		RenderCommand::Init();
		Renderer2D::Init();
	}

	void Renderer::Shutdown()
	{
		Renderer2D::Shutdown();
	}

	void Renderer::OnWindowResize(uint32_t _width, uint32_t _height)
	{
		RenderCommand::SetViewport(0, 0, _width, _height);
	}

	void Renderer::BeginScene(const OrthographicCamera& _camera)
	{
		s_sceneData.ViewProjectionMatrix = _camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const std::shared_ptr<VertexArray>& _vertexArray, const std::shared_ptr<Material>& _material, const glm::mat4& _transform)
	{
		_material->Bind();

		_material->GetShader()->SetMat4("u_viewProjection", s_sceneData.ViewProjectionMatrix);
		_material->GetShader()->SetMat4("u_transform", _transform);

		_vertexArray->Bind();
		RenderCommand::DrawIndexed(_vertexArray);
	}
}