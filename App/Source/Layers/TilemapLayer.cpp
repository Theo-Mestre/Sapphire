#include "TilemapLayer.h"

TilemapLayer::TilemapLayer()
{
}

TilemapLayer::~TilemapLayer()
{
}

void TilemapLayer::OnAttach()
{
	m_tileMap = sph::TileMap::Create("TileSheet/TileMapConfig.txt");

	m_tilemapRenderer = sph::CreateScope<sph::TilemapRenderer>();
	m_tilemapRenderer->Init();

	ASSERT(m_camera != nullptr, "Camera is not set!");
}

void TilemapLayer::OnDetach()
{
	m_tilemapRenderer->Shutdown();
}

void TilemapLayer::OnUpdate(sph::DeltaTime _dt)
{
}

void TilemapLayer::OnRender(const sph::Ref<sph::Renderer>& _renderer)
{
	m_tilemapRenderer->BeginScene(*m_camera);
	{
		m_tilemapRenderer->DrawTileMap(m_tileMap);
	}
	m_tilemapRenderer->EndScene();
}

void TilemapLayer::OnImGuiRender()
{
}

void TilemapLayer::OnEvent(sph::Event& _event)
{
}
