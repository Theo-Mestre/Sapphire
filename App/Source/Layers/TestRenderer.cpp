#include "TestRenderer.h"

TestRenderer::TestRenderer(sph::Application& _app)
	: Layer("TestRenderer")
	, m_tileMapData()
	, m_app(_app)
{
	m_app.GetWindow().SetVSync(false);
}

void TestRenderer::OnAttach()
{
	m_cameraController = sph::CreateScope<sph::OrthographicCameraController>(1280.0f / 720.0f, true);

	m_texture = sph::Texture2D::Create("TileSheet.png");

	m_subTexture[0] = sph::SubTexture2D::Create(m_texture, { 4, 1 }, { 16, 16 });

	sph::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });

	LoadTileMap();
}

void TestRenderer::OnDetach()
{
}

void TestRenderer::OnUpdate(sph::DeltaTime _dt)
{
	PROFILE_FUNCTION();

	m_cameraController->OnUpdate(_dt);

	sph::Renderer2D::s_stats.Reset();
	sph::RenderCommand::Clear();

	sph::Renderer2D::BeginScene(m_cameraController->GetCamera());
	{
		//sph::Renderer2D::DrawQuad({ 0.0f,0.0f, 0.0f }, { 1.0f, 1.0f }, m_subTexture);
		const float tileSize = (60.0f / 1920.0f);

		for (uint32_t y = 0; y < MAP_SIZE_Y; y++)
		{
			for (uint32_t x = 0; x < MAP_SIZE_X; x++)
			{
				int32_t tileIndex = m_tileMapData[y * MAP_SIZE_X + x];
				if (tileIndex == -1)
					continue;

				glm::vec2 position = { x * tileSize, 1 - y * tileSize };
				sph::Renderer2D::DrawQuad(position, glm::vec2{ tileSize, tileSize }, m_subTexture[0]);
			}
		}
	}
	sph::Renderer2D::EndScene();
}

void TestRenderer::OnImGuiRender()
{
	PROFILE_FUNCTION();

	ImGui::Begin("Settings");
	{
		ImGui::ColorEdit4("Color", glm::value_ptr(m_color));
	}
	ImGui::End();

	ImGui::Begin("Stats");
	{
		ImGui::Text("frame time : %f", sph::Time::DeltaTime);
		ImGui::Text("fps : %d", int(1 / sph::Time::DeltaTime));
		ImGui::Text("quad number : %d", sph::Renderer2D::s_stats.QuadCount);
		ImGui::Text("draw call : %d", sph::Renderer2D::s_stats.DrawCalls);
	}
	ImGui::End();
}

void TestRenderer::OnEvent(sph::Event& _event)
{
	m_cameraController->OnEvent(_event);
}

void TestRenderer::LoadTileMap()
{
	std::ifstream file("TileSetData.csv");
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
	LogDebug("TileMap loaded");
}
