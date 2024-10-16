#include "TestRenderer.h"

TestRenderer::TestRenderer(sph::Application* const _app)
	: Layer("TestRenderer")
	, m_tileMapData()
	, m_app(_app)
{
}

void TestRenderer::OnAttach()
{
	m_cameraController = sph::CreateScope<sph::OrthographicCameraController>(1.0f, true);

	m_renderer = sph::CreateScope<sph::Renderer2D>();
	m_renderer->Init();
	m_renderer->SetScreenSize({ (float)m_app->GetWindow().GetWidth(), (float)m_app->GetWindow().GetHeight() });
	m_texture = sph::Texture2D::Create("TileMap.png");

	sph::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });

	// AppData buffer
	float data[2] = { (float)m_app->GetWindow().GetWidth(), (float)m_app->GetWindow().GetHeight() };
	m_appDataUniformBuffer = sph::UniformBuffer::Create({ { sph::ShaderDataType::Float2, "Resolution"} }, 0);
	m_appDataUniformBuffer->SetData(data, sizeof(data));

	LoadTileMap();
}

void TestRenderer::OnDetach()
{
	m_renderer->Shutdown();
}

void TestRenderer::OnUpdate(sph::DeltaTime _dt)
{
	m_cameraController->OnUpdate(_dt);
}

void TestRenderer::OnRender(const sph::Ref<sph::Renderer>& _renderer)
{
	sph::Renderer2D::Stats::Reset();
	sph::RenderCommand::Clear();

	_renderer->BeginScene(m_cameraController->GetCamera());
	{
		const float tileSize = 48.0f;
		for (uint32_t y = 0; y < MAP_SIZE_Y; y++)
		{
			for (uint32_t x = 0; x < MAP_SIZE_X; x++)
			{
				int tileValue = m_tileMapData[y * MAP_SIZE_X + x];
				if (tileValue == -1) continue;

				_renderer->DrawQuad({ x * tileSize,(MAP_SIZE_Y - 1 - y) * tileSize, 0.0f }, { tileSize * 2, tileSize * 2 }, m_subTexture[tileValue]);
			}
		}
	}
	_renderer->EndScene();


	m_renderer->BeginScene(m_cameraController->GetCamera());
	{
		m_renderer->DrawQuad({ 700.0f, 500.0f, 0.0f }, m_texture->GetSize(), m_texture);

		m_renderer->DrawQuad({ 0.0f, 0.0f, 0.0f }, { 10, 10 }, 0.0f, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		m_renderer->DrawQuad({ 1280.f, 780.0f, 0.0f }, { 10, 10 }, 0.0f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		m_renderer->DrawQuad({ 0.0f, 780.0f, 0.0f }, { 10, 10 }, 0.0f, glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
		m_renderer->DrawQuad({ 1280.f, 0.0f, 0.0f }, { 10, 10 }, 0.0f, glm::vec4(0.0f, 1.0f, 1.0f, 1.0f));
	}
	m_renderer->EndScene();
}

void TestRenderer::OnImGuiRender()
{

	auto mousePos = sph::Input::GetMousePosition();
	auto vp = m_cameraController->GetCamera().GetViewProjectionMatrix();

	mousePos.x = (mousePos.x / m_app->GetWindow().GetWidth()) * 2.0f - 1.0f;
	mousePos.y = (mousePos.y / m_app->GetWindow().GetHeight()) * 2.0f - 1.0f;

	glm::vec4 worldPos = glm::inverse(vp) * glm::vec4(mousePos, 0.0f, 1.0f);

	ImGui::Begin("Debug");
	ImGui::Text("Mouse Position: %f, %f", worldPos.x, worldPos.y);
	ImGui::End();
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