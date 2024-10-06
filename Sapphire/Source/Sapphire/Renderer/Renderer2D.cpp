#include "sphpch.h"
#include "Renderer2D.h"

#include "Sapphire/Renderer/Shader.h"
#include "Sapphire/Renderer/VertexArray.h"
#include "Sapphire/Renderer/Buffer.h"
#include "Sapphire/Renderer/Camera.h"
#include "Sapphire/Renderer/RenderCommand.h"
#include "Sapphire/Renderer/SubTexture2D.h"
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
		static const uint32_t maxQuads = 10000;
		static const uint32_t maxVertices = maxQuads * 4;
		static const uint32_t maxIndices = maxQuads * 6;
		static const uint32_t maxTextureSlots = 32;
		static constexpr glm::vec2 defaultTexCoords[4] =
		{
			{ 0.0f, 0.0f },
			{ 1.0f, 0.0f },
			{ 1.0f, 1.0f },
			{ 0.0f, 1.0f }
		};

		uint32_t quadIndexCount = 0;
		QuadVertex* quadVertexBufferBase = nullptr;
		QuadVertex* quadVertexBufferPointer = nullptr;
		glm::vec4 quadTransform[4];

		Ref<VertexArray> vertexArray;
		Ref<VertexBuffer> vertexBuffer;
		Ref<Shader> shader;
		Ref<Texture2D> whiteTexture;

		// TODO: Use Assets Handle
		std::array<Ref<Texture2D>, maxTextureSlots> textureSlots;
		uint32_t textureSlotIndex = 1;
	};

	static Renderer2DData s_data;


	static void UpdateCurrentQuadVertex(const glm::vec3& _position, const glm::vec2& _size, float _rotation, const glm::vec4& _color, float _texID, float _tilingFactor, const glm::vec2* _texCoords = s_data.defaultTexCoords)
	{
		constexpr uint32_t vertexCount = 4;

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), _position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(_rotation), { 0.0f, 0.0f, 1.0f }) *
			glm::scale(glm::mat4(1.0f), { _size.x, _size.y, 1.0f });

		for (uint32_t i = 0; i < vertexCount; i++)
		{
			s_data.quadVertexBufferPointer->position = transform * s_data.quadTransform[i];
			s_data.quadVertexBufferPointer->color = _color;
			s_data.quadVertexBufferPointer->texIndex = _texID;
			s_data.quadVertexBufferPointer->texCoord = _texCoords[i];
			s_data.quadVertexBufferPointer->tilingFactor = _tilingFactor;
			s_data.quadVertexBufferPointer++;
		}

		s_data.quadIndexCount += 6;
	}
	static void ResetBatchStates()
	{
		s_data.quadIndexCount = 0;
		s_data.quadVertexBufferPointer = s_data.quadVertexBufferBase;
		s_data.textureSlotIndex = 1;
	}

	void Renderer2D::Init()
	{
		PROFILE_FUNCTION();
		// Vertex Array Creattion
		s_data.vertexArray = VertexArray::Create();

		// Vertex Buffer Creation
		s_data.vertexBuffer = VertexBuffer::Create(s_data.maxVertices * sizeof(QuadVertex));

		// Buffer Layout
		BufferLayout layout =
		{
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" },
			{ ShaderDataType::Float2, "a_TexCoord" },
			{ ShaderDataType::Float, "a_TexIndex" },
			{ ShaderDataType::Float, "a_TilingFactor" }
		};
		s_data.vertexBuffer->SetLayout(layout);
		s_data.vertexArray->AddVertexBuffer(s_data.vertexBuffer);

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
		s_data.vertexArray->SetIndexBuffer(indexBuffer);
		delete[] indices;

		// White Texture
		s_data.whiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_data.whiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));
		s_data.textureSlots[0] = s_data.whiteTexture;

		// Shader
		int32_t samplers[s_data.maxTextureSlots];
		for (int32_t i = 0; i < s_data.maxTextureSlots; i++)
		{
			samplers[i] = i;
		}

		s_data.shader = Shader::Create("Shaders/Texture.glsl");
		s_data.shader->Bind();
		s_data.shader->SetIntArray("u_textures", samplers, s_data.maxTextureSlots);

		// Quad Vertex Buffer
		s_data.quadVertexBufferBase = new QuadVertex[s_data.maxVertices];

		s_stats.DrawCalls = 0;

		s_data.quadTransform[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_data.quadTransform[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		s_data.quadTransform[2] = { 0.5f, 0.5f, 0.0f, 1.0f };
		s_data.quadTransform[3] = { -0.5f, 0.5f, 0.0f, 1.0f };
	}

	void Renderer2D::Shutdown()
	{
		delete[] s_data.quadVertexBufferBase;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& _camera)
	{
		s_data.vertexArray->Bind();
		s_data.shader->Bind();
		s_data.shader->SetMat4("u_viewProjection", _camera.GetViewProjectionMatrix());

		ResetBatchStates();
	}

	void Renderer2D::EndScene()
	{
		Flush();
	}

	void Renderer2D::Flush()
	{
		//// Bind all the textures
		for (uint32_t i = 0; i < s_data.textureSlotIndex; i++)
		{
			s_data.textureSlots[i]->Bind(i);
		}

		// Send the data of the current batch to the GPU
		uint64_t dataSize = (uint8_t*)s_data.quadVertexBufferPointer - (uint8_t*)s_data.quadVertexBufferBase;
		s_data.vertexBuffer->SetData(s_data.quadVertexBufferBase, dataSize);

		// Draw the vertices
		RenderCommand::DrawIndexed(s_data.vertexArray, s_data.quadIndexCount);
		s_stats.DrawCalls++;

		ResetBatchStates();
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
		if (s_data.quadIndexCount >= s_data.maxIndices)
		{
			Flush();
		}

		float textureIndex = 0.0f;
		UpdateCurrentQuadVertex(_position, _size, _rotation, _color, textureIndex, 1.0f);

		s_stats.QuadCount++;
	}

	void Renderer2D::DrawQuad(const glm::vec2& _position, const glm::vec2& _size, const Ref<Texture2D>& _texture)
	{
		DrawQuad({ _position.x, _position.y, 0.0f }, _size, _texture);
	}

	void Renderer2D::DrawQuad(const glm::vec3& _position, const glm::vec2& _size, const Ref<Texture2D>& _texture)
	{
		if (s_data.quadIndexCount >= s_data.maxIndices)
		{
			Flush();
		}

		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < s_data.textureSlotIndex; i++)
		{
			if (*s_data.textureSlots[i] == *_texture)
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			if (s_data.textureSlotIndex >= s_data.maxTextureSlots)
			{
				Flush();
				s_data.textureSlotIndex = 0;
			}

			textureIndex = (float)s_data.textureSlotIndex;
			s_data.textureSlots[s_data.textureSlotIndex] = _texture;
			s_data.textureSlotIndex++;
		}

		UpdateCurrentQuadVertex(_position, _size, 0.0f, { 1.0f, 1.0f, 1.0f, 1.0f }, textureIndex, 1.0f);

		s_stats.QuadCount++;
	}

	void Renderer2D::DrawQuad(const glm::vec3& _position, const glm::vec2& _size, const Ref<SubTexture2D>& _subTexture)
	{
		if (s_data.quadIndexCount >= s_data.maxIndices)
		{
			Flush();
		}

		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < s_data.textureSlotIndex; i++)
		{
			if (*s_data.textureSlots[i] == *(_subTexture->GetTexture()))
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			if (s_data.textureSlotIndex >= s_data.maxTextureSlots)
			{
				Flush();
				s_data.textureSlotIndex = 0;
			}

			textureIndex = (float)s_data.textureSlotIndex;
			s_data.textureSlots[s_data.textureSlotIndex] = _subTexture->GetTexture();
			s_data.textureSlotIndex++;
		}

		UpdateCurrentQuadVertex(_position, _size, 0.0f, { 1.0f, 1.0f, 1.0f, 1.0f }, textureIndex, 1.0f, _subTexture->GetTexCoords());

		s_stats.QuadCount++;
	}

	void Renderer2D::Stats::Reset()
	{
		memset(&s_stats, 0, sizeof(Renderer2D::Stats));
	}
}