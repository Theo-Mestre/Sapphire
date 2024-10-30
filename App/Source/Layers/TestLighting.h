#pragma once

#include "Sapphire.h"

#include "Light.h"

constexpr auto MAP_SIZE_X = 40;
constexpr auto MAP_SIZE_Y = 25;

struct TileLayer
{
	TileLayer() = default;
	TileLayer(const sph::Ref<sph::Texture2D>& _tilemapTexture)
		: tilemapTexture(_tilemapTexture)
	{
	}

	std::array<int32_t, MAP_SIZE_X* MAP_SIZE_Y> tiles = std::array<int32_t, MAP_SIZE_X* MAP_SIZE_Y>();
	sph::Ref<sph::Texture2D> tilemapTexture = nullptr;
	std::unordered_map<uint32_t, sph::Ref<sph::SubTexture2D>> subTextures = std::unordered_map<uint32_t, sph::Ref<sph::SubTexture2D>>();
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

	sph::Ref<sph::Sprite> m_sprite = nullptr;

	// TileMap
	sph::Ref<sph::Texture2D> m_tilemapTexture;
	sph::Ref<sph::Texture2D> m_waterTilemapTexture;
	std::unordered_map<uint32_t, sph::Ref<sph::SubTexture2D>> m_subTexture;
	std::array<int32_t, MAP_SIZE_X * MAP_SIZE_Y> m_tileMapData;

	std::vector<TileLayer> m_tileLayers;

	sph::Ref<sph::TileMap> m_tileMap;
	sph::Scope<sph::TilemapRenderer> m_tilemapRenderer = nullptr;

	bool m_enableLight = false;
};