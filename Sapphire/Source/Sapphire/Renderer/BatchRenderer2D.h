#ifndef BATCH_RENDERER_2D_H
#define BATCH_RENDERER_2D_H

#include <Sapphire/Renderer/Renderer.h>

namespace sph
{
	class OrthographicCamera;
	class VertexArray;
	class VertexBuffer;
	class Shader;
	struct QuadVertex;

	class BatchRenderer2D
		: public Renderer
	{
	public:
		BatchRenderer2D() = default;
		virtual ~BatchRenderer2D() override;

		virtual void Init() override;
		virtual void Shutdown() override;

		virtual void BeginScene(const OrthographicCamera& _camera) override;
		virtual void EndScene() override;
		void Flush();

		// Rendering Submition Functions
		virtual void DrawQuad(const glm::vec3& _position, const glm::vec2& _size, float _rotation, const glm::vec4& _color) override;
		virtual void DrawQuad(const glm::vec3& _position, const glm::vec2& _size, const Ref<Texture2D>& _texture) override;
		virtual void DrawQuad(const glm::vec3& _position, const glm::vec2& _size, const Ref<SubTexture2D>& _subTexture) override;

		static Ref<Renderer> Create();

	private:
		void InitShader();

		void ResetBatchStates();
		void UpdateCurrentQuadVertex(const glm::vec3& _position, const glm::vec2& _size, float _rotation, const glm::vec4& _color, float _texID, float _tilingFactor, const glm::vec2* _texCoords = defaultTexCoords);

		float SubmitTexture(const Ref<Texture2D>& _texture);
	private:
		uint32_t quadIndexCount = 0;
		QuadVertex* quadVertexBufferBase = nullptr;
		QuadVertex* quadVertexBufferPointer = nullptr;
		glm::vec4 quadTransform[4] = { {} };

		Ref<VertexArray> vertexArray;
		Ref<VertexBuffer> vertexBuffer;
		Ref<Shader> shader;
		Ref<Texture2D> whiteTexture;


		static const uint32_t maxQuads = 10000;
		static const uint32_t maxVertices = maxQuads * 4;
		static const uint32_t maxIndices = maxQuads * 6;
		static const uint32_t maxTextureSlots = 32; // GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS
		static constexpr glm::vec2 defaultTexCoords[4] =
		{
			{ 0.0f, 0.0f },
			{ 1.0f, 0.0f },
			{ 1.0f, 1.0f },
			{ 0.0f, 1.0f }
		};

		std::array<Ref<Texture2D>, maxTextureSlots> textureSlots;
		uint32_t textureSlotIndex = 1;
	};
}

#endif