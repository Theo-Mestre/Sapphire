#ifndef SPH_RENDERER_H
#define SPH_RENDERER_H

#include "Renderer.h"

namespace sph
{
	class OrthographicCamera;
	class Camera;
	class Texture2D;
	class SubTexture2D;
	class VertexArray;
	class Shader;

	class Renderer2D
		: public Renderer
	{
	public:
		Renderer2D();
		virtual ~Renderer2D() = default;

		virtual void Init() override;
		virtual void Shutdown() override;

		virtual void BeginScene(const glm::mat4& _viewProjection) override;
		virtual void BeginScene(const Camera& _camera, const glm::mat4& _transform) override;
		virtual void EndScene() override;

		virtual void DrawQuad(const glm::mat4& _transform, const glm::vec4& _color) override;
		virtual void DrawQuad(const glm::vec3& _position, const glm::vec2& _size, float _rotation, const glm::vec4& _color) override;
		virtual void DrawQuad(const glm::vec3& _position, const glm::vec2& _size, const Ref<Texture2D>& _texture) override;
		virtual void DrawQuad(const glm::vec3& _position, const glm::vec2& _size, const Ref<SubTexture2D>& _subTexture) override;
		virtual void DrawQuad(const glm::mat4& _transform, const Ref<Texture2D>& _texture, const glm::vec4& _color = glm::vec4(1.0f)) override;

		void DrawQuad(const glm::vec3& _position, const glm::vec2& _size, const Ref<Texture2D>& _texture, const Ref<Shader>& _shader);
		void DrawQuad(const glm::vec3& _position, const glm::vec2& _size, const uint32_t _textureID);

		virtual void DrawSprite(const glm::mat4& _transform, const SpriteRendererComponent& _sprite, int32_t _entityID = -1) override;

		static Ref<Renderer> Create();
	private:
		Ref<VertexArray> m_vertexArray;
		Ref<Texture2D> m_whiteTexture;
		Ref<Shader> m_shader;
	};
}
#endif