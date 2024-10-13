#pragma once

#include "Sapphire.h"

struct Light
{
	glm::vec2 position;
	glm::vec3 color;
	float intensity;
	float radius;
};


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
	sph::Scope<sph::OrthographicCameraController> m_cameraController = nullptr;

	sph::Scope<sph::Renderer2D> m_renderer2D;
	glm::vec3 m_ambiantLightColor;

	sph::Ref<sph::Shader> m_lightShader;
	sph::Ref<sph::Texture2D> m_texture;

	int blockSize;
	uint32_t m_UBO;
};