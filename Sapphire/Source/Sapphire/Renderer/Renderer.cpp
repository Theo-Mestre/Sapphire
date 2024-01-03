#include "sphpch.h"
#include "Renderer.h"
#include "Shader.h"
#include "VertexArray.h"

namespace sph
{
	void Renderer::Init()
	{
	}

	void Renderer::OnWindowResize(uint32_t _width, uint32_t _height)
	{
	}

	void Renderer::BeginScene()
	{
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const std::shared_ptr<VertexArray>& _vertexArray, const std::shared_ptr<Shader>& _shader)
	{
		_shader->Bind();
		_vertexArray->Bind();
		RenderCommand::DrawIndexed(_vertexArray);
	}
}