#include "TestLighting.h"

TestLighting::TestLighting(sph::Application* const _app)
	: Layer("TestLighting")
	, m_app(_app)
	, m_lightData()
	, m_ambiantLightColor(1.0f)
	, m_tileMapData()
	, m_postFXProjection(0.0f)
{
}

TestLighting::~TestLighting()
{
}

void TestLighting::OnAttach()
{
	// Camera
	glm::vec2 halfSize = m_app->GetWindow().GetSize() / 2.0f;
	m_camera = sph::OrthographicCamera::Create(-halfSize.x, halfSize.x, -halfSize.y, halfSize.y);
	m_cameraController = sph::CreateScope<sph::OrthographicCameraController>(m_camera);

	m_postFXProjection = glm::ortho(-halfSize.x, halfSize.x, -halfSize.y, halfSize.y, -1.0f, 1.0f);

	// Renderer
	m_renderer2D = sph::CreateScope<sph::Renderer2D>();
	m_renderer2D->Init();
	sph::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });

	// Texture
	m_texture = sph::Texture2D::Create("Background.png");
	m_playerTexture = sph::Texture2D::Create("Player.png");

	// Framebuffer
	m_framebuffer = sph::Framebuffer::Create({ (uint32_t)m_app->GetWindow().GetWidth(), (uint32_t)m_app->GetWindow().GetHeight() });

	// AppData buffer
	glm::vec2 data = m_app->GetWindow().GetSize();
	m_appDataUniformBuffer = sph::UniformBuffer::Create({ { sph::ShaderDataType::Float2, "Resolution"} }, 0);
	m_appDataUniformBuffer->SetData(&data, sizeof(glm::vec2));

	// Create Lights
	m_lightData.UploadLightsData(
		{
			{ { 0.96078431, 0.8156862745098039, 0.12156862745098039 , 1.0f}, { 100.0f, 100.0f }, 1.0f, 0.3f },
			{ { 1.0f, 0.8f, .8f , 1.0f}, { -500.0f, -200.0f }, 1.f, 0.3f },
		});

	m_ambiantLightColor = { 0.1f, 0.1f, 0.2f };

	// tilemap
	m_tilemapTexture = sph::Texture2D::Create("TileMap.png");
	m_waterTilemapTexture = sph::Texture2D::Create("WaterToGrass.png");


	// Sprite
	auto texture = sph::Texture2D::Create("Sprite.png");
	m_sprite = sph::Sprite::Create(texture, true);
	m_sprite->SetSize({ 500.0f, 500.0f });
	m_sprite->SetOffset(glm::vec2(0.5f));

	m_tileLayers.reserve(4);

	// TileLayer
	m_tileLayers.emplace_back(m_tilemapTexture); // grass

	m_tileLayers.emplace_back(m_waterTilemapTexture); // water

	m_tileLayers.emplace_back(m_tilemapTexture); // detail

	m_tileLayers.emplace_back(sph::Texture2D::Create("Details2.png")); // detail2	

	LoadTileMap();

	m_tileMap = sph::TileMap::Create("TileSheet/TileMapConfig.txt");
	m_tilemapRenderer = sph::CreateScope<sph::TilemapRenderer>();
	m_tilemapRenderer->Init();
}

void TestLighting::OnDetach()
{
	m_renderer2D->Shutdown();
	m_tilemapRenderer->Shutdown();
}

void TestLighting::OnUpdate(sph::DeltaTime _dt)
{
	m_cameraController->OnUpdate(_dt);

	glm::vec2 mousePosition = m_camera->ScreenToWorld(sph::Input::GetMousePosition());
	m_sprite->SetPosition(mousePosition);
	m_lightData.GetUniformBuffer()->SetData(&mousePosition, sizeof(glm::vec2), 32);
}

void TestLighting::OnRender(const sph::Ref<sph::Renderer>& _renderer)
{
	sph::Renderer::Stats::Reset();
	sph::RenderCommand::Clear();

	m_framebuffer->Bind();
	m_framebuffer->Clear();

	m_tilemapRenderer->BeginScene(*m_camera);
	{
		m_tilemapRenderer->DrawTileMap(m_tileMap);
	}
	m_tilemapRenderer->EndScene();

	_renderer->BeginScene(*m_camera);
	{
		//for (auto& layer : m_tileLayers)
		//{
		//	const float tileSize = 32;
		//	const glm::vec2 mapOffest = { tileSize * (MAP_SIZE_X / 2.0f), tileSize * (MAP_SIZE_Y / 2.0f) };
		//
		//	for (uint32_t y = 0; y < MAP_SIZE_Y; y++)
		//	{
		//		for (uint32_t x = 0; x < MAP_SIZE_X; x++)
		//		{
		//			int32_t tileIndex = layer.tiles[y * MAP_SIZE_X + x];
		//			if (tileIndex == -1) continue;
		//
		//			glm::vec3 position = { x * tileSize - mapOffest.x ,1 - y * tileSize + mapOffest.y, 0.0f };
		//			_renderer->DrawQuad(position, glm::vec2{ tileSize, tileSize }, layer.subTextures[tileIndex]);
		//		}
		//	}
		//}

		_renderer->DrawQuad({ 1280.0f, 0.0f, 0.0f }, m_app->GetWindow().GetSize(), m_texture);
		_renderer->DrawQuad({ 100.0f, 100.0f, 0.0f }, { 100.0f, 100.0f }, m_playerTexture);
		_renderer->DrawSprite(*m_sprite);
	}
	_renderer->EndScene();
	m_framebuffer->Unbind();

	m_renderer2D->BeginScene(m_postFXProjection);
	{
		if (m_enableLight == false)
		{
			m_renderer2D->DrawQuad({ 0.0f, 0.0f, 0.0f }, m_app->GetWindow().GetSize(), m_framebuffer->GetTextureAttachment());
			m_renderer2D->EndScene();
			return;
		}

		auto& lightShader = m_lightData.GetShader();
		lightShader->Bind();
		lightShader->SetMat4("u_viewProjection", m_postFXProjection);
		lightShader->SetMat4("u_sceneVP", m_camera->GetViewProjectionMatrix());
		lightShader->SetFloat3("u_ambientLight", m_ambiantLightColor);

		m_renderer2D->DrawQuad({ 0.0f, 0.0f, 0.0f }, m_app->GetWindow().GetSize(), m_framebuffer->GetTextureAttachment(), lightShader);
	}
	m_renderer2D->EndScene();
}

void TestLighting::OnImGuiRender()
{
	ImGui::Begin("Lighting");
	{
		ImGui::ColorEdit3("Ambiant Light Color", glm::value_ptr(m_ambiantLightColor));

		ImGui::Checkbox("Toggle Lights", &m_enableLight);

	}
	ImGui::End();
}

void TestLighting::OnEvent(sph::Event& _event)
{
	m_cameraController->OnEvent(_event);
}

void TestLighting::LoadTileMap()
{
	glm::vec2 cellSize = { 32, 32 };
	glm::vec2 cellNumber[4] =
	{
		{55, 72},
		{30, 29},
		{55, 72},
		{55, 72}
	};

	const char* tilemap[4] =
	{
		"TileSheet/Map_Grass.csv",
		"TileSheet/Map_Water.csv",
		"TileSheet/Map_Details.csv",
		"TileSheet/Map_Detail2.csv"
	};

	uint32_t cellNumberIndex = 0;
	for (auto& layer : m_tileLayers)
	{
		std::ifstream file(tilemap[cellNumberIndex]);
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
				layer.tiles[row * MAP_SIZE_X + col] = std::stoi(cell);
				col++;
			}
			row++;
		}
		file.close();

		for (auto value : layer.tiles)
		{
			if (value == -1) continue;

			glm::vec2 index = { value % (int)cellNumber[cellNumberIndex].x, cellNumber[cellNumberIndex].y - (value / (int)cellNumber[cellNumberIndex].x) - 1 };
			layer.subTextures[value] = sph::SubTexture2D::Create(layer.tilemapTexture, index, cellSize);
		}

		cellNumberIndex++;
	}
}