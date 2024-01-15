#include "sphpch.h"
#include "Renderer.h"

#include "Shader.h"
#include "Camera.h"
#include "Material.h"
#include "VertexArray.h"
#include "Sapphire/Platform/OpenGL/OpenGLShader.h"

namespace sph
{
	Renderer::SceneData Renderer::s_sceneData = Renderer::SceneData();

	void Renderer::Init()
	{
		RenderCommand::Init();
		//Renderer2D::Init();

	}

	void Renderer::OnWindowResize(uint32_t _width, uint32_t _height)
	{
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

		sph::OpenGLShader* shader = static_cast<sph::OpenGLShader*>(_material->GetShader().get());
		shader->SetUniformMat4f("u_viewProjection", s_sceneData.ViewProjectionMatrix);
		shader->SetUniformMat4f("u_transform", _transform);

		_vertexArray->Bind();
		RenderCommand::DrawIndexed(_vertexArray);
	}
}