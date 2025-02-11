#include "sphpch.h"
#include "Renderer2D.h"
#include "Sapphire/Renderer/VertexArray.h"
#include "Sapphire/Renderer/Buffer.h"
#include "Sapphire/Renderer/Shader.h"
#include "Sapphire/Renderer/RenderCommand.h"
#include "Sapphire/Renderer/SubTexture2D.h"
#include "Sapphire/Renderer/Camera.h"
#include "Sapphire/Renderer/Sprite.h"
#include "Sapphire/Core/Log.h"

namespace sph
{
	Renderer2D::Renderer2D()
		: m_vertexArray(nullptr)
		, m_shader(nullptr)
		, m_whiteTexture(nullptr)
	{
		SPH_PROFILE_FUNCTION();

	}

	void Renderer2D::Init()
	{
		SPH_PROFILE_FUNCTION();

		float vertices[4 * 5] = {
		   -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		   0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		   0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
		   -0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		uint32_t indices[6] =
		{ 
			0, 1, 2, 
			0, 2, 3 
		};

		BufferLayout layout =
		{
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" }
		};

		Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices));

		Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
		
		m_vertexArray = VertexArray::Create();
		vertexBuffer->SetLayout(layout);
		m_vertexArray->AddVertexBuffer(vertexBuffer);
		m_vertexArray->SetIndexBuffer(indexBuffer);
		
		m_shader = Shader::Create("Shaders/SingleQuad.glsl");
		m_shader->Bind();
		m_shader->SetInt("u_texture", 0);
		
		m_whiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		m_whiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));
	}

	void Renderer2D::Shutdown()
	{
		SPH_PROFILE_FUNCTION();

		m_vertexArray.reset();
		m_shader.reset();
		m_whiteTexture.reset();
	}

	void Renderer2D::BeginScene(const OrthographicCamera& _camera)
	{
		SPH_PROFILE_FUNCTION();

		m_shader->Bind();
		m_shader->SetMat4("u_viewProjection", _camera.GetViewProjectionMatrix());
	}

	void Renderer2D::BeginScene(const glm::mat4& _viewProjection)
	{
		SPH_PROFILE_FUNCTION();

		m_shader->Bind();
		m_shader->SetMat4("u_viewProjection", _viewProjection);
	}

	void Renderer2D::BeginScene(const Camera& _camera, const glm::mat4& _transform)
	{
		SPH_PROFILE_FUNCTION();

		m_shader->Bind();
		m_shader->SetMat4("u_viewProjection", _camera.GetProjection() * glm::inverse(_transform));
	}

	void Renderer2D::EndScene()
	{
		SPH_PROFILE_FUNCTION();

	}

	void Renderer2D::DrawQuad(const glm::mat4& _transform, const glm::vec4& _color)
	{
		SPH_PROFILE_FUNCTION();

		m_shader->SetMat4("u_transform", _transform);
		m_shader->SetFloat4("u_color", _color);

		m_whiteTexture->Bind();
		m_vertexArray->Bind();
		RenderCommand::DrawIndexed(m_vertexArray);
		Renderer::Stats::DrawCalls++;
		Renderer::Stats::QuadCount++;
	}

	void Renderer2D::DrawQuad(const glm::vec3& _position, const glm::vec2& _size, float _rotation, const glm::vec4& _color)
	{
		SPH_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), _position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(_rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { _size.x, _size.y, 1.0f });

		m_shader->SetMat4("u_transform", transform);
		m_shader->SetFloat4("u_color", _color);

		m_whiteTexture->Bind();
		m_vertexArray->Bind();
		RenderCommand::DrawIndexed(m_vertexArray);
		Renderer::Stats::DrawCalls++;
		Renderer::Stats::QuadCount++;
	}

	void Renderer2D::DrawQuad(const glm::vec3& _position, const glm::vec2& _size, const Ref<Texture2D>& _texture)
	{
		SPH_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), _position)
			* glm::scale(glm::mat4(1.0f), { _size.x, _size.y, 1.0f });

		m_shader->Bind();
		m_shader->SetMat4("u_transform", transform);
		m_shader->SetFloat4("u_color", glm::vec4(1.0f));

		_texture->Bind();
		m_vertexArray->Bind();
		RenderCommand::DrawIndexed(m_vertexArray);
		Renderer::Stats::DrawCalls++;
		Renderer::Stats::QuadCount++;
	}

	void Renderer2D::DrawQuad(const glm::vec3& _position, const glm::vec2& _size, const Ref<Texture2D>& _texture, const Ref<Shader>& _shader)
	{
		SPH_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), _position)
			* glm::scale(glm::mat4(1.0f), { _size.x, _size.y, 1.0f });

		_texture->Bind();
		_shader->Bind();
		_shader->SetMat4("u_transform", transform);
		m_shader->SetFloat4("u_color", glm::vec4(1.0f));
		m_vertexArray->Bind();
		RenderCommand::DrawIndexed(m_vertexArray);
		Renderer::Stats::DrawCalls++;
		Renderer::Stats::QuadCount++;

		_shader->Unbind();
	}

	void Renderer2D::DrawQuad(const glm::vec3& _position, const glm::vec2& _size, const uint32_t _textureID)
	{
		SPH_PROFILE_FUNCTION();

		m_shader->Bind();
		m_shader->SetMat4("u_transform", glm::translate(glm::mat4(1.0f), _position)
			* glm::scale(glm::mat4(1.0f), { _size.x, _size.y, 1.0f }));
		m_shader->SetFloat4("u_color", glm::vec4(1.0f));

		m_vertexArray->Bind();
		RenderCommand::DrawIndexed(m_vertexArray);
		Renderer::Stats::DrawCalls++;
		Renderer::Stats::QuadCount++;
	}

	void Renderer2D::DrawSprite(const Sprite& _sprite)
	{
		SPH_PROFILE_FUNCTION();

		m_shader->Bind();
		m_shader->SetMat4("u_transform", _sprite.GetTransform());
		m_shader->SetFloat4("u_color", _sprite.GetColor());

		_sprite.GetTexture()->Bind();
		m_vertexArray->Bind();
		RenderCommand::DrawIndexed(m_vertexArray);
		Renderer::Stats::DrawCalls++;
		Renderer::Stats::QuadCount++;
	}

	void Renderer2D::DrawQuad(const glm::vec3& _position, const glm::vec2& _size, const Ref<SubTexture2D>& _texture)
	{
		SPH_PROFILE_FUNCTION();

		LogWarn("Renderer2D doesn't support SubTexture!");

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), _position)
			* glm::scale(glm::mat4(1.0f), { _size.x, _size.y, 1.0f });

		m_shader->Bind();
		m_shader->SetMat4("u_transform", transform);
		m_shader->SetFloat4("u_color", glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));

		m_whiteTexture->Bind();
		m_vertexArray->Bind();
		RenderCommand::DrawIndexed(m_vertexArray);
		Renderer::Stats::DrawCalls++;
		Renderer::Stats::QuadCount++;
	}

	Ref<Renderer> Renderer2D::Create()
	{
		return CreateRef<Renderer2D>();
	}
}