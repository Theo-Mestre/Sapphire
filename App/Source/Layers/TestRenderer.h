#pragma once 

#include "Sapphire.h"

class TestRenderer
	: public sph::Layer
{
public:
	TestRenderer();
	~TestRenderer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(sph::DeltaTime _dt) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(sph::Event& _event) override;
private:
	glm::vec4 m_color = { 1.0f, 1.0f, 1.0f, 1.0f };
	sph::Scope<sph::OrthographicCameraController> m_cameraController = nullptr;

	sph::Ref<sph::Texture2D> m_texture;
	sph::Ref<sph::SubTexture2D> m_subTexture;

	float m_rotation = 0.0f;
};