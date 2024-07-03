#pragma once 

#include "Sapphire.h"

namespace sph
{
	class Shader;
	class VertexArray;
	class Texture2D;
}

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

	sph::Ref<sph::Texture2D> m_texture;

	sph::Scope<sph::OrthographicCameraController> m_cameraController = nullptr;

	float m_rotation = 0.0f;
};