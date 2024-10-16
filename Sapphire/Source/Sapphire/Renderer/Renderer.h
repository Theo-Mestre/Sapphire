#ifndef RENDERER_H
#define RENDERER_H

#include <Sapphire/Core/Core.h>

namespace sph
{
	class OrthographicCamera;
	class Texture2D;
	class SubTexture2D;

	class Renderer
	{
	public:
		Renderer() = default;
		virtual ~Renderer() = default;

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
			inline static uint32_t DrawCalls;
			inline static uint32_t QuadCount;

			static void Reset()
			{
				DrawCalls = 0;
				QuadCount = 0;
			}
		};

		inline void SetScreenSize(glm::vec2 _screenSize) { m_screenSize = _screenSize; }
	protected:
		glm::vec3 NormalizePosition(const glm::vec3& _position);
		glm::vec2 NormalizeSize(const glm::vec2& _size);
	protected:
		glm::vec2 m_screenSize = { 0.0f, 0.0f };
	};
}
#endif