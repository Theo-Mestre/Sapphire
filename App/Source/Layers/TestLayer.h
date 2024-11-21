#pragma once

#include "Sapphire.h"

class TestLayer
	: public sph::Layer
{
public:
	TestLayer();
	~TestLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(sph::DeltaTime _dt) override;
	virtual void OnRender(const sph::Ref<sph::Renderer>& _renderer) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(sph::Event& _event) override;

	void SetCamera(const sph::Ref<sph::OrthographicCamera>& _camera) { m_camera = _camera; }

private:
	sph::Ref<sph::OrthographicCamera> m_camera = nullptr;
	sph::Scope<sph::OrthographicCameraController> m_cameraController = nullptr;

	sph::Ref<sph::UniformBuffer> m_appDataUniformBuffer = nullptr;
	sph::Ref<sph::Framebuffer> m_framebuffer = nullptr;

	sph::Ref<sph::Texture2D> m_texture = nullptr;
	sph::Ref<sph::Texture2D> m_playerTexture = nullptr;

	sph::Ref<sph::Sprite> m_sprite = nullptr;
};