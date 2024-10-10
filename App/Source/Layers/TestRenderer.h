#pragma once 

#include "Sapphire.h"

#define MAP_SIZE_X 120
#define MAP_SIZE_Y 65
#define MAX_SUB_TEXTURE 4

class TestRenderer
	: public sph::Layer
{
public:
	TestRenderer(sph::Application& _app);
	~TestRenderer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(sph::DeltaTime _dt) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(sph::Event& _event) override;
private:
	void LoadTileMap();
private:
	sph::Application& m_app;

	glm::vec4 m_color = { 1.0f, 1.0f, 1.0f, 1.0f };
	sph::Scope<sph::OrthographicCameraController> m_cameraController = nullptr;

	sph::Ref<sph::Texture2D> m_texture;
	std::array<sph::Ref<sph::SubTexture2D>, MAX_SUB_TEXTURE> m_subTexture;

	float m_rotation = 0.0f;

	std::array<int32_t, MAP_SIZE_X * MAP_SIZE_Y> m_tileMapData;
};