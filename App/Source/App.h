#pragma once

#include "Sapphire/Application.h"

namespace sph
{
	class OrthographicCamera;
	class VextexArray;
	class Texture2D;
	class Material;
	class Window;
	class Shader;
}

class Sandbox
	: public sph::Application
{
public:
	Sandbox();
	virtual ~Sandbox() = default;

private:
	virtual void Init() override;
	virtual void OnEvent(sph::Event& _event) override;
	virtual void OnUpdate() override;
	virtual void OnRender() override;
	virtual void OnImGuiRender() override;

	void InitShaders();

private:
	sph::Ref<sph::VertexArray> m_spriteVA;
	sph::Ref<sph::VertexArray> m_rectVA;
	sph::Ref<sph::VertexArray> m_triVA;
	sph::Ref<sph::Material> m_defaultMaterial;
	sph::Ref<sph::Material> m_colorMaterial;
	sph::Ref<sph::Material> m_textureMaterial;
	sph::Ref<sph::Texture2D> m_texture;
	sph::Ref<sph::Texture2D> m_sapphire;
	sph::Ref<sph::Framebuffer> m_framebuffer;
	glm::mat4 m_squareTransform;

	glm::vec4 m_color1;
	glm::vec4 m_color2;

	sph::Scope<sph::OrthographicCamera> m_camera;
};
