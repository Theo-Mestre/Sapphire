#include "sphpch.h"
#include "BatchRenderer2D.h"

#include "Sapphire/Renderer/Renderer2D.h"
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

	BatchRenderer2D::~BatchRenderer2D()
	{
		ASSERT(quadVertexBufferBase != nullptr, "BatchRenderer: Shutdown must be called before destroy the renderer!");
	}

	void BatchRenderer2D::Init()
	{
		PROFILE_FUNCTION();
		RenderCommand::Init();

		// Vertex Array Creattion
		vertexArray = VertexArray::Create();

		// Vertex Buffer Creation
		vertexBuffer = VertexBuffer::Create(maxVertices * sizeof(QuadVertex));

		// Buffer Layout
		BufferLayout layout =
		{
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" },
			{ ShaderDataType::Float2, "a_TexCoord" },
			{ ShaderDataType::Float, "a_TexIndex" },
			{ ShaderDataType::Float, "a_TilingFactor" }
		};
		vertexBuffer->SetLayout(layout);
		vertexArray->AddVertexBuffer(vertexBuffer);

		// Index Buffer
		uint32_t* indices = new uint32_t[maxIndices];
		uint32_t offset = 0;
		for (uint32_t i = 0; i < maxIndices; i += 6)
		{
			indices[i + 0] = offset + 0;
			indices[i + 1] = offset + 1;
			indices[i + 2] = offset + 2;

			indices[i + 3] = offset + 2;
			indices[i + 4] = offset + 3;
			indices[i + 5] = offset + 0;

			offset += 4;
		}
		Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(indices, maxIndices);
		vertexArray->SetIndexBuffer(indexBuffer);
		delete[] indices;

		// White Texture
		whiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		whiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));
		textureSlots[0] = whiteTexture;

		InitShader();


		// Quad Vertex Buffer
		quadVertexBufferBase = new QuadVertex[maxVertices];

		quadTransform[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		quadTransform[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		quadTransform[2] = { 0.5f, 0.5f, 0.0f, 1.0f };
		quadTransform[3] = { -0.5f, 0.5f, 0.0f, 1.0f };
	}

	void BatchRenderer2D::Shutdown()
	{
		delete[] quadVertexBufferBase;
		quadVertexBufferBase = nullptr;
	}

	void BatchRenderer2D::BeginScene(const OrthographicCamera& _camera)
	{
		vertexArray->Bind();
		shader->Bind();
		shader->SetMat4("u_viewProjection", _camera.GetViewProjectionMatrix());

		ResetBatchStates();
	}

	void BatchRenderer2D::EndScene()
	{
		Flush();
	}

	void BatchRenderer2D::Flush()
	{
		//// Bind all the textures
		for (uint32_t i = 0; i < textureSlotIndex; i++) { textureSlots[i]->Bind(i); }

		// Send the data of the current batch to the GPU
		uint64_t dataSize = (uint8_t*)quadVertexBufferPointer - (uint8_t*)quadVertexBufferBase;
		vertexBuffer->SetData(quadVertexBufferBase, dataSize);

		// Draw the vertices
		RenderCommand::DrawIndexed(vertexArray, quadIndexCount);
		Renderer2D::s_stats.DrawCalls++;

		ResetBatchStates();
	}

	void BatchRenderer2D::DrawQuad(const glm::vec3& _position, const glm::vec2& _size, float _rotation, const glm::vec4& _color)
	{
		if (quadIndexCount >= maxIndices)
		{
			Flush();
		}

		float textureIndex = 0.0f;
		UpdateCurrentQuadVertex(_position, _size, _rotation, _color, textureIndex, 1.0f);

		Renderer2D::s_stats.QuadCount++;
	}

	void BatchRenderer2D::DrawQuad(const glm::vec3& _position, const glm::vec2& _size, const Ref<Texture2D>& _texture)
	{
		if (quadIndexCount >= maxIndices)
		{
			Flush();
		}

		float textureIndex = SubmitTexture(_texture);

		UpdateCurrentQuadVertex(_position, _size, 0.0f, { 1.0f, 1.0f, 1.0f, 1.0f }, textureIndex, 1.0f);

		Renderer2D::s_stats.QuadCount++;
	}

	void BatchRenderer2D::DrawQuad(const glm::vec3& _position, const glm::vec2& _size, const Ref<SubTexture2D>& _subTexture)
	{
		if (quadIndexCount >= maxIndices)
		{
			Flush();
		}

		float textureIndex = SubmitTexture(_subTexture->GetTexture());
		
		UpdateCurrentQuadVertex(_position, _size, 0.0f, { 1.0f, 1.0f, 1.0f, 1.0f }, textureIndex, 1.0f, _subTexture->GetTexCoords());

		Renderer2D::s_stats.QuadCount++;
	}

	Ref<Renderer> BatchRenderer2D::Create()
	{
		return CreateRef<BatchRenderer2D>();
	}

	void BatchRenderer2D::InitShader()
	{
		int32_t samplers[maxTextureSlots] = { 0 };
		for (int32_t i = 0; i < maxTextureSlots; i++)
		{
			samplers[i] = i;
		}

		shader = Shader::Create("Shaders/Texture.glsl");
		shader->Bind();
		shader->SetIntArray("u_textures", samplers, maxTextureSlots);
	}

	void BatchRenderer2D::ResetBatchStates()
	{
		quadIndexCount = 0;
		quadVertexBufferPointer = quadVertexBufferBase;
		textureSlotIndex = 1;
	}

	void BatchRenderer2D::UpdateCurrentQuadVertex(const glm::vec3& _position, const glm::vec2& _size, float _rotation, const glm::vec4& _color, float _texID, float _tilingFactor, const glm::vec2* _texCoords)
	{
		constexpr uint32_t vertexCount = 4;

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), _position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(_rotation), { 0.0f, 0.0f, 1.0f }) *
			glm::scale(glm::mat4(1.0f), { _size.x, _size.y, 1.0f });

		for (uint32_t i = 0; i < vertexCount; i++)
		{
			quadVertexBufferPointer->position = transform * quadTransform[i];
			quadVertexBufferPointer->color = _color;
			quadVertexBufferPointer->texIndex = _texID;
			quadVertexBufferPointer->texCoord = _texCoords[i];
			quadVertexBufferPointer->tilingFactor = _tilingFactor;
			quadVertexBufferPointer++;
		}

		quadIndexCount += 6;
	}

	float BatchRenderer2D::SubmitTexture(const Ref<Texture2D>& _texture)
	{
		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < textureSlotIndex; i++)
		{
			if (*textureSlots[i] == *(_texture))
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			if (textureSlotIndex >= maxTextureSlots)
			{
				Flush();
				textureSlotIndex = 0;
			}

			textureIndex = (float)textureSlotIndex;
			textureSlots[textureSlotIndex] = _texture;
			textureSlotIndex++;
		}

		return textureIndex;
	}
}