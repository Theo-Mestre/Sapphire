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
		Renderer2D() = default;
		virtual ~Renderer2D() = default;

		virtual void Init() = 0;
		virtual void Shutdown() = 0;

		virtual void BeginScene(const OrthographicCamera& _camera) = 0;
		virtual void EndScene() = 0;

		virtual void DrawQuad(const glm::vec3& _position, const glm::vec2& _size, float _rotation, const glm::vec4& _color) = 0;
		virtual void DrawQuad(const glm::vec3& _position, const glm::vec2& _size, const Ref<Texture2D>& _texture) = 0;
		virtual void DrawQuad(const glm::vec3& _position, const glm::vec2& _size, const Ref<SubTexture2D>& _subTexture) = 0;

		void OnWindowResize(uint32_t _width, uint32_t _height);

		struct Stats
		{
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;

			void Reset()
			{
				memset(this, 0, sizeof(Stats));	
			}
		};

		inline static Stats s_stats;
	};
}
#endif