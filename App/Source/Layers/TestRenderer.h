#pragma once 

#include "Sapphire.h"

#include <unordered_map>

constexpr auto MAP_SIZE_X = 30;
constexpr auto MAP_SIZE_Y = 20;
constexpr auto MAX_SUB_TEXTURE = 4;

struct Sprite
{
	glm::vec3 position;
	glm::vec2 size;
	glm::vec4 color;
	sph::Ref<sph::Texture2D> texture;
};

class TestRenderer
	: public sph::Layer
{
public:
	TestRenderer(sph::Application* const _app);
	~TestRenderer() = default;

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

	glm::vec4 m_color = { 1.0f, 1.0f, 1.0f, 1.0f };
	sph::Ref<sph::OrthographicCamera> m_camera;
	sph::Ref<sph::Texture2D> m_texture;
	Sprite m_player;
	sph::Ref<sph::Texture2D> m_playerTexture;
	std::unordered_map<uint32_t, sph::Ref<sph::SubTexture2D>> m_subTexture;

	sph::Ref<sph::Framebuffer> m_framebuffer;

	float m_rotation = 0.0f;

	std::array<int32_t, MAP_SIZE_X * MAP_SIZE_Y> m_tileMapData;
};