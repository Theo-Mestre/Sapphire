#include "TestRenderer.h"

TestRenderer::TestRenderer(sph::Application* const _app)
	: Layer("TestRenderer")
	, m_tileMapData()
	, m_app(_app)
{
}

void TestRenderer::OnAttach()
{
	glm::vec2 halfSize = m_app->GetWindow().GetSize() / 2.0f;
	m_camera = sph::CreateRef<sph::OrthographicCamera>(-halfSize.x, halfSize.x, -halfSize.y, halfSize.y);

	m_texture = sph::Texture2D::Create("TileMap.png");
	m_playerTexture = sph::Texture2D::Create("Player.png");

	m_player.position = glm::vec3(0.0f);
	m_player.color = glm::vec4(0.0f);
	m_player.size = glm::vec2(100.0f);
	m_player.texture = m_playerTexture;

	m_framebuffer = sph::Framebuffer::Create(sph::FramebufferSpecification{ 1280, 720 });

	sph::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });

	LoadTileMap();
}

void TestRenderer::OnDetach()
{
}

void TestRenderer::OnUpdate(sph::DeltaTime _dt)
{
	sph::KeyCode::KeyCode inputs[] =
	{
		sph::KeyCode::W,
		sph::KeyCode::A,
		sph::KeyCode::S,
		sph::KeyCode::D,
		sph::KeyCode::Q,
		sph::KeyCode::E
	};

	glm::vec3 zoom = m_camera->GetRotation();

	// move camera depending on the camera
	for (uint32_t i = 0; i < 6; i++)
	{
		if (sph::Input::IsKeyPressed(inputs[i]))
		{
			switch (inputs[i])
			{
			case sph::KeyCode::W:
				m_player.position.y += 500 * _dt;
				break;
			case sph::KeyCode::S:
				m_player.position.y -= 500 * _dt;
				break;
			case sph::KeyCode::A:
				m_player.position.x -= 500 * _dt;
				break;
			case sph::KeyCode::D:
				m_player.position.x += 500 * _dt;
				break;
			case sph::KeyCode::Q:
				zoom += 90.0f * _dt;
				break;
			case sph::KeyCode::E:
				zoom -= 90.0f * _dt;
				break;
			default:
				break;
			}
		}
	}
	m_camera->SetPosition(m_player.position);
	m_camera->SetRotation(zoom);
}

void TestRenderer::OnRender(const sph::Ref<sph::Renderer>& _renderer)
{
	sph::Renderer2D::s_stats.Reset();
	sph::RenderCommand::Clear();

	m_framebuffer->Bind();
	m_framebuffer->Clear();
	_renderer->BeginScene(*m_camera);
	{

		const float tileSize = 48;
		const glm::vec2 mapOffest = { tileSize * (MAP_SIZE_X / 2.0f), tileSize * (MAP_SIZE_Y / 2.0f) };

		for (uint32_t y = 0; y < MAP_SIZE_Y; y++)
		{
			for (uint32_t x = 0; x < MAP_SIZE_X; x++)
			{
				int32_t tileIndex = m_tileMapData[y * MAP_SIZE_X + x];
				if (tileIndex == -1) continue;

				glm::vec3 position = { x * tileSize - mapOffest.x ,1 - y * tileSize + mapOffest.y, 0.0f };
				_renderer->DrawQuad(position, glm::vec2{ tileSize, tileSize }, m_subTexture[tileIndex]);
			}
		}

		_renderer->DrawQuad(m_player.position, m_player.size, m_player.texture);

	}
	_renderer->EndScene();
	m_framebuffer->Unbind();

	glm::vec2 halfSize = m_app->GetWindow().GetSize() / 2.0f;
	static sph::OrthographicCamera camera(-halfSize.x, halfSize.x, -halfSize.y, halfSize.y);

	_renderer->BeginScene(camera);
	{
		_renderer->DrawQuad(glm::vec3{ 0.0f, 0.0f, 0.0f }, m_app->GetWindow().GetSize(), m_framebuffer->GetTextureAttachment());
	}
	_renderer->EndScene();
}

void TestRenderer::OnImGuiRender()
{
}

void TestRenderer::OnEvent(sph::Event& _event)
{
	//m_cameraController->OnEvent(_event);
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