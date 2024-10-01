#include "sphpch.h"
#include "Renderer2D.h"

#include "Sapphire/Renderer/Shader.h"
#include "Sapphire/Renderer/VertexArray.h"
#include "Sapphire/Renderer/Buffer.h"
#include "Sapphire/Renderer/Camera.h"
#include "Sapphire/Renderer/RenderCommand.h"
#include "Sapphire/Renderer/Texture.h"
#include "Sapphire/Profiling/Profiler.h"
#include "Sapphire/Core/Log.h"

namespace sph
{
	struct QuadVertex
	{
		glm::vec3 position;
		glm::vec4 color;
		glm::vec2 texCoord;
		float texIndex;
		float tilingFactor;
	};

	struct Renderer2DData
	{
		const uint32_t maxQuads = 10000;
		const uint32_t maxVertices = maxQuads * 4;
		const uint32_t maxIndices = maxQuads * 6;
		const uint32_t maxTextureSlots = 32;

		uint32_t quadIndexCount = 0;
		QuadVertex* quadVertexBufferBase = nullptr;
		QuadVertex* quadVertexBufferPointer = nullptr;

		Ref<VertexArray> vertexArray;
		Ref<VertexBuffer> vertexBuffer;
		Ref<Shader> textureShader;
		Ref<Texture2D> whiteTexture;
	};

	static Renderer2DData s_data;

	void Renderer2D::Init()
	{
		PROFILE_FUNCTION();

		s_data.quadVertexBufferBase = new QuadVertex[s_data.maxVertices];

		// Index Buffer
		uint32_t* indices = new uint32_t[s_data.maxIndices];
		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_data.maxIndices; i += 6)
		{
			indices[i + 0] = offset + 0;
			indices[i + 1] = offset + 1;
			indices[i + 2] = offset + 2;
			indices[i + 3] = offset + 2;
			indices[i + 4] = offset + 3;
			indices[i + 5] = offset + 0;

			offset += 4;
		}
		Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(indices, s_data.maxIndices);
		delete[] indices;

		// Index Buffer Layout
		BufferLayout layout =
		{
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" },
			{ ShaderDataType::Float2, "a_TexCoord" },
			{ ShaderDataType::Float, "a_TexIndex" },
			{ ShaderDataType::Float, "a_TilingFactor" }
		};

		s_data.vertexBuffer = VertexBuffer::Create(s_data.maxQuads * sizeof(QuadVertex));
		s_data.vertexBuffer->SetLayout(layout);

		// Vertex Array
		s_data.vertexArray = VertexArray::Create();
		s_data.vertexArray->AddVertexBuffer(s_data.vertexBuffer);
		s_data.vertexArray->SetIndexBuffer(indexBuffer);

		// Shader
		s_data.textureShader = Shader::Create("Shaders/Texture.glsl");
		s_data.textureShader->Bind();

		// White Texture
		s_data.whiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_data.whiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		DrawCalls = 0;
	}

	void Renderer2D::Shutdown()
	{
	}

	void Renderer2D::BeginScene(const OrthographicCamera& _camera)
	{
		s_data.textureShader->Bind();
		s_data.textureShader->SetMat4("u_viewProjection", _camera.GetViewProjectionMatrix());

		s_data.quadIndexCount = 0;
		s_data.quadVertexBufferPointer = s_data.quadVertexBufferBase;
		DrawCalls = 0;
	}

	void Renderer2D::EndScene()
	{
		uint32_t dataSize = (uint8_t*)s_data.quadVertexBufferPointer - (uint8_t*)s_data.quadVertexBufferBase;
		s_data.vertexBuffer->SetData(s_data.quadVertexBufferBase, dataSize);

		Flush();
	}

	void Renderer2D::Flush()
	{
		RenderCommand::DrawIndexed(s_data.vertexArray, s_data.quadIndexCount);
		DrawCalls++;
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
		s_data.quadVertexBufferPointer->position = { _position.x, _position.y, _position.z };
		s_data.quadVertexBufferPointer->color = _color;
		s_data.quadVertexBufferPointer->texCoord = { 0.0f, 0.0f };
		s_data.quadVertexBufferPointer->texIndex = 0.0f;
		s_data.quadVertexBufferPointer->tilingFactor = 1.0f;
		s_data.quadVertexBufferPointer++;

		s_data.quadVertexBufferPointer->position = { _position.x + _size.x, _position.y, _position.z };
		s_data.quadVertexBufferPointer->color = _color;
		s_data.quadVertexBufferPointer->texCoord = { 1.0f, 0.0f };
		s_data.quadVertexBufferPointer->texIndex = 0.0f;
		s_data.quadVertexBufferPointer->tilingFactor = 1.0f;
		s_data.quadVertexBufferPointer++;

		s_data.quadVertexBufferPointer->position = { _position.x + _size.x, _position.y + _size.y, _position.z };
		s_data.quadVertexBufferPointer->color = _color;
		s_data.quadVertexBufferPointer->texCoord = { 1.0f, 1.0f };
		s_data.quadVertexBufferPointer->texIndex = 0.0f;
		s_data.quadVertexBufferPointer->tilingFactor = 1.0f;
		s_data.quadVertexBufferPointer++;

		s_data.quadVertexBufferPointer->position = { _position.x, _position.y + _size.y, _position.z };
		s_data.quadVertexBufferPointer->color = _color;
		s_data.quadVertexBufferPointer->texCoord = { 0.0f, 1.0f };
		s_data.quadVertexBufferPointer->texIndex = 0.0f;
		s_data.quadVertexBufferPointer->tilingFactor = 1.0f;
		s_data.quadVertexBufferPointer++;

		s_data.quadIndexCount += 6;
		//glm::mat4 transform = glm::translate(glm::mat4(1.0f), _position)
		//	* glm::rotate(glm::mat4(1.0f), glm::radians(_rotation), { 0.0f, 0.0f, 1.0f })
		//	* glm::scale(glm::mat4(1.0f), { _size.x, _size.y, 1.0f });
		//
		//s_data.textureShader->SetMat4("u_transform", transform);
		//s_data.textureShader->SetFloat4("u_color", _color);
		//
		//s_data.whiteTexture->Bind();
		//s_data.vertexArray->Bind();
		//RenderCommand::DrawIndexed(s_data.vertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec2& _position, const glm::vec2& _size, const Ref<Texture2D>& _texture)
	{
		DrawQuad({ _position.x, _position.y, 0.0f }, _size, _texture);
	}

	void Renderer2D::DrawQuad(const glm::vec3& _position, const glm::vec2& _size, const Ref<Texture2D>& _texture)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), _position)
			* glm::scale(glm::mat4(1.0f), { _size.x, _size.y, 1.0f });

		s_data.textureShader->SetFloat4("u_color", glm::vec4(1.0f));
		s_data.textureShader->SetMat4("u_transform", transform);

		_texture->Bind();

		s_data.vertexArray->Bind();
		RenderCommand::DrawIndexed(s_data.vertexArray);
	}
}