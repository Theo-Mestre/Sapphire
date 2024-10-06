#ifndef RENDERER_2D_H
#define RENDERER_2D_H

#include <Sapphire/Core/Core.h>

namespace sph
{
	class Texture2D;
	class SubTexture2D;
	class OrthographicCamera;

	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& _camera);
		static void EndScene();
		static void Flush();

		static void DrawQuad(const glm::vec2& _position, const glm::vec2& _size, const glm::vec4& _color);
		static void DrawQuad(const glm::vec2& _position, const glm::vec2& _size, float _rotation, const glm::vec4& _color);
		static void DrawQuad(const glm::vec3& _position, const glm::vec2& _size, const glm::vec4& _color);
		static void DrawQuad(const glm::vec3& _position, const glm::vec2& _size, float _rotation, const glm::vec4& _color);
		static void DrawQuad(const glm::vec2& _position, const glm::vec2& _size, const Ref<Texture2D>& _texture);
		static void DrawQuad(const glm::vec3& _position, const glm::vec2& _size, const Ref<Texture2D>& _texture);
		static void DrawQuad(const glm::vec3& _position, const glm::vec2& _size, const Ref<SubTexture2D>& _subTexture);

		struct Stats
		{
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;

			static void Reset();
		};

		inline static Stats s_stats;
	};
}
#endif