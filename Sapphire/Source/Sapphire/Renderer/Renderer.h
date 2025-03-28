#ifndef RENDERER_H
#define RENDERER_H

#include <glm/vec4.hpp>

#include <Sapphire/Core/Core.h>

namespace sph
{
	class OrthographicCamera;
	class Camera;
	class Texture2D;
	class SubTexture2D;
	struct SpriteRendererComponent;

	class Renderer
	{
	public:
		Renderer() = default;
		virtual ~Renderer() = default;

		virtual void Init() = 0;
		virtual void Shutdown() = 0;

		virtual void BeginScene(const glm::mat4& _viewProjection) = 0;
		virtual void BeginScene(const Camera& _camera, const glm::mat4& _transform) = 0;
		virtual void EndScene() = 0;

		virtual void DrawQuad(const glm::mat4& _transform, const glm::vec4& _color) = 0;
		virtual void DrawQuad(const glm::vec3& _position, const glm::vec2& _size, float _rotation, const glm::vec4& _color) = 0;
		virtual void DrawQuad(const glm::vec3& _position, const glm::vec2& _size, const Ref<Texture2D>& _texture) = 0;
		virtual void DrawQuad(const glm::vec3& _position, const glm::vec2& _size, const Ref<SubTexture2D>& _subTexture) = 0;
		virtual void DrawQuad(const glm::mat4& _transform, const Ref<Texture2D>& _texture, const glm::vec4& _color = glm::vec4(1.0f)) = 0;

		virtual void DrawSprite(const glm::mat4& _transform, const SpriteRendererComponent& _sprite, int32_t _entityID = -1) = 0;

		void OnWindowResize(uint32_t _width, uint32_t _height);

		inline static struct Stats
		{
			inline static uint32_t DrawCalls = 0;
			inline static uint32_t QuadCount = 0;

			inline static void Reset()
			{
				DrawCalls = 0;
				QuadCount = 0;
			}
		} Stats;
	};
}
#endif