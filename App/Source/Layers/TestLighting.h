#pragma once

#include "Sapphire.h"

#include "Light.h"

class TestLighting
	: public sph::Layer
{
public:
	TestLighting(sph::Application* const _app);
	~TestLighting();

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(sph::DeltaTime _dt) override;
	virtual void OnRender(const sph::Ref<sph::Renderer>& _renderer) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(sph::Event& _event) override;
private:
	sph::Application* const m_app;
	sph::Ref<sph::OrthographicCamera> m_camera;

	sph::Ref<sph::Framebuffer> m_framebuffer;

	sph::Scope<sph::Renderer2D> m_renderer2D;
	sph::Ref<sph::UniformBuffer> m_appDataUniformBuffer;
	glm::vec3 m_ambiantLightColor;
	sph::Ref<sph::Texture2D> m_texture;
	sph::Ref<sph::Texture2D> m_playerTexture;

	LightData m_lightData;

	glm::vec2 m_mousePosition;
};