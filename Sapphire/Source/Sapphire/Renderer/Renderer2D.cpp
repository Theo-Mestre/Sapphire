#include "sphpch.h"
#include "Renderer2D.h"

#include "Sapphire/Renderer/Shader.h"
#include "Sapphire/Renderer/VertexArray.h"
#include "Sapphire/Renderer/Buffer.h"
#include "Sapphire/Renderer/Camera.h"
#include "Sapphire/Renderer/RenderCommand.h"
#include "Sapphire/Renderer/Texture.h"

namespace sph
{
	struct Renderer2DData
	{
		Ref<VertexArray> vertexArray;
		Ref<Shader> textureShader;
		Ref<Texture2D> whiteTexture;
	};

	static Renderer2DData* s_data;

	void Renderer2D::Init()
	{
		s_data = new Renderer2DData();

		float vertices[4 * 5] = {
		   -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		   0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		   0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
		   -0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices));

		uint32_t indices[6] = { 0, 1, 2, 0, 2, 3 };
		Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));

		BufferLayout layout = 
		{ 
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" }
		};

		s_data->vertexArray = VertexArray::Create();
		vertexBuffer->SetLayout(layout);
		s_data->vertexArray->AddVertexBuffer(vertexBuffer);
		s_data->vertexArray->SetIndexBuffer(indexBuffer);

		s_data->textureShader = Shader::Create("Shaders/Texture.glsl");
		s_data->textureShader->Bind();
		s_data->textureShader->SetInt("u_texture", 0);

		s_data->whiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_data->whiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));
		
	}

	void Renderer2D::Shutdown()
	{
		delete s_data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& _camera)
	{		
		s_data->textureShader->Bind();
		s_data->textureShader->SetMat4("u_viewProjection", _camera.GetViewProjectionMatrix());
	}

	void Renderer2D::EndScene()
	{

	}

	void Renderer2D::DrawQuad(const glm::vec2& _position, const glm::vec2& _size, const glm::vec4& _color)
	{
		DrawQuad({ _position.x, _position.y, 0.0f }, _size, 0.0f, _color);
	}

	void Renderer2D::DrawQuad(const glm::vec2& _position, const glm::vec2& _size, float _rotation, const glm::vec4& _color)
	{
		DrawQuad({ _position.x, _position.y, 0.0f }, _size, _rotation, _color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& _position, const glm::vec2& _size, const glm::vec4& _color)
	{
		DrawQuad(_position, _size, 0.0f, _color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& _position, const glm::vec2& _size, float _rotation, const glm::vec4& _color)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), _position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(_rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { _size.x, _size.y, 1.0f });

		s_data->textureShader->SetMat4("u_transform", transform);
		s_data->textureShader->SetFloat4("u_color", _color);

		s_data->whiteTexture->Bind();
		s_data->vertexArray->Bind();
		RenderCommand::DrawIndexed(s_data->vertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec2& _position, const glm::vec2& _size, const Ref<Texture2D>& _texture)
	{
		DrawQuad({ _position.x, _position.y, 0.0f }, _size, _texture);
	}

	void Renderer2D::DrawQuad(const glm::vec3& _position, const glm::vec2& _size, const Ref<Texture2D>& _texture)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), _position)
			* glm::scale(glm::mat4(1.0f), { _size.x, _size.y, 1.0f });

		s_data->textureShader->SetFloat4("u_color", glm::vec4(1.0f));
		s_data->textureShader->SetMat4("u_transform", transform);

		_texture->Bind();

		s_data->vertexArray->Bind();
		RenderCommand::DrawIndexed(s_data->vertexArray);
	}
}