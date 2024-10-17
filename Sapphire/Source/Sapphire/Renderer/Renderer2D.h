#ifndef SPH_RENDERER_H
#define SPH_RENDERER_H

#include "Renderer.h"

namespace sph
{
	class Texture2D;
	class SubTexture2D;
	class OrthographicCamera;
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

		virtual void BeginScene(const OrthographicCamera& _camera) override;
		virtual void EndScene() override;

		virtual void DrawQuad(const glm::vec3& _position, const glm::vec2& _size, float _rotation, const glm::vec4& _color) override;
		virtual void DrawQuad(const glm::vec3& _position, const glm::vec2& _size, const Ref<Texture2D>& _texture) override;
		virtual void DrawQuad(const glm::vec3& _position, const glm::vec2& _size, const Ref<SubTexture2D>& _subTexture) override;
		void DrawQuad(const glm::vec3& _position, const glm::vec2& _size, const Ref<Texture2D>& _texture, const Ref<Shader>& _shader);

		static Ref<Renderer> Create();

	private:
		Ref<VertexArray> m_vertexArray;
		Ref<Texture2D> m_whiteTexture;
		Ref<Shader> m_shader;
	};
}
#endif