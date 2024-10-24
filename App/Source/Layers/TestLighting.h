#pragma once

#include "Sapphire.h"

#include "Light.h"

constexpr auto MAP_SIZE_X = 30;
constexpr auto MAP_SIZE_Y = 20;

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
	void LoadTileMap();

private:
	sph::Application* const m_app;

	sph::Ref<sph::OrthographicCamera> m_camera = nullptr;
	sph::Scope<sph::OrthographicCameraController> m_cameraController = nullptr;

	// Renderer2D for postFX
	sph::Scope<sph::Renderer2D> m_renderer2D = nullptr;

	sph::Ref<sph::UniformBuffer> m_appDataUniformBuffer = nullptr;
	sph::Ref<sph::Framebuffer> m_framebuffer = nullptr;

	sph::Ref<sph::Texture2D> m_texture = nullptr;
	sph::Ref<sph::Texture2D> m_playerTexture = nullptr;

	LightData m_lightData;
	glm::vec3 m_ambiantLightColor;

	glm::mat4 m_postFXProjection;

	// TileMap
	sph::Ref<sph::Texture2D> m_tilemapTexture;
	std::unordered_map<uint32_t, sph::Ref<sph::SubTexture2D>> m_subTexture;
	std::array<int32_t, MAP_SIZE_X * MAP_SIZE_Y> m_tileMapData;
};