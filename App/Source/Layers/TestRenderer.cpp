#include "TestRenderer.h"

TestRenderer::TestRenderer(sph::Application* const _app)
	: Layer("TestRenderer")
	, m_tileMapData()
	, m_app(_app)
{
}

void TestRenderer::OnAttach()
{
	m_cameraController = sph::CreateScope<sph::OrthographicCameraController>(1280.0f / 720.0f, true);

	m_texture = sph::Texture2D::Create("TileMap.png");

	sph::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });

	LoadTileMap();
}

void TestRenderer::OnDetach()
{
}

void TestRenderer::OnUpdate(sph::DeltaTime _dt)
{
	m_cameraController->OnUpdate(_dt);
}

void TestRenderer::OnRender(const sph::Ref<sph::Renderer>& _renderer)
{
	sph::Renderer2D::s_stats.Reset();
	sph::RenderCommand::Clear();

	_renderer->BeginScene(m_cameraController->GetCamera());
	{
		const float tileSize = (60.0f / 1920.0f);

		for (uint32_t y = 0; y < MAP_SIZE_Y; y++)
		{
			for (uint32_t x = 0; x < MAP_SIZE_X; x++)
			{
				int32_t tileIndex = m_tileMapData[y * MAP_SIZE_X + x];
				if (tileIndex == -1) continue;

				glm::vec3 position = { x * tileSize + glm::epsilon<float>(), 1 - y * tileSize + glm::epsilon<float>(), 0.0f};
				_renderer->DrawQuad(position, glm::vec2{ tileSize, tileSize }, m_subTexture[tileIndex]);
			}
		}
	}
	_renderer->EndScene();
}

void TestRenderer::OnImGuiRender()
{
}

void TestRenderer::OnEvent(sph::Event& _event)
{
	m_cameraController->OnEvent(_event);
}

void TestRenderer::LoadTileMap()
{
	std::ifstream file("Demo.csv");
	if (!file.is_open())
	{
		ASSERT(false, "Failed to open file");
		return;
	}

	std::string line;
	uint32_t row = 0;
	while (std::getline(file, line))
	{
		std::stringstream ss(line);
		std::string cell;
		uint32_t col = 0;
		while (std::getline(ss, cell, ','))
		{
			m_tileMapData[row * MAP_SIZE_X + col] = std::stoi(cell);
			col++;
		}
		row++;
	}
	file.close();

	glm::vec2 cellSize = { 16, 16 };
	glm::vec2 cellNumber = { 22, 6 };

	for (auto value : m_tileMapData)
	{
		if (value == -1) continue;

		glm::vec2 index = { value % (int)cellNumber.x, cellNumber.y - (value / (int)cellNumber.x) };
		m_subTexture[value] = sph::SubTexture2D::Create(m_texture, index, cellSize);
	}
}