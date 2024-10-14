#pragma once

#include "Sapphire.h"

struct Light
{
	glm::vec4 color;
	glm::vec2 position;
	float intensity;
	float radius;
};

struct LightData
{
	int lightCount = 0; // Allign to 16 bytes
	Light lights[100]; 
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


	sph::Ref<sph::UniformBuffer> m_ubo;
	int blockSize;
	uint32_t m_UBOhandmade;

	uint32_t m_ssbo;

	glm::vec2 m_mousePosition;
};