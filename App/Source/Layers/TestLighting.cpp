#include "TestLighting.h"

TestLighting::TestLighting()
	: Layer("TestLighting")
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
	glm::vec2 halfSize = m_application->GetWindow().GetSize() / 2.0f;
	m_camera = sph::OrthographicCamera::Create(-halfSize.x, halfSize.x, -halfSize.y, halfSize.y);
	m_cameraController = sph::CreateScope<sph::OrthographicCameraController>(m_camera, true);

	m_postFXProjection = glm::ortho(-halfSize.x, halfSize.x, -halfSize.y, halfSize.y, -1.0f, 1.0f);

	// Renderer
	m_renderer2D = sph::CreateScope<sph::Renderer2D>();
	m_renderer2D->Init();
	sph::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });

	// Texture
	m_texture = sph::Texture2D::Create("Background.png");
	m_playerTexture = sph::Texture2D::Create("Player.png");

	// Framebuffer
	m_framebuffer = sph::Framebuffer::Create({ (uint32_t)m_application->GetWindow().GetWidth(), (uint32_t)m_application->GetWindow().GetHeight() });

	// AppData buffer
	glm::vec2 data = m_application->GetWindow().GetSize();
	m_appDataUniformBuffer = sph::UniformBuffer::Create({ { sph::ShaderDataType::Float2, "Resolution"} }, 0);
	m_appDataUniformBuffer->SetData(&data, sizeof(glm::vec2));

	// Create Lights
	m_lightData.UploadLightsData(
		{
			{ { 0.96078431, 0.8156862745098039, 0.12156862745098039 , 1.0f}, { 100.0f, 100.0f }, 1.0f, 0.3f },
			{ { 1.0f, 0.8f, .8f , 1.0f}, { -500.0f, -200.0f }, 1.f, 0.3f },
		});

	m_ambiantLightColor = { 0.1f, 0.1f, 0.2f };

	// Sprite
	auto texture = sph::Texture2D::Create("Sprite.png");
	m_sprite = sph::Sprite::Create(texture, true);
	m_sprite->SetSize({ 500.0f, 500.0f });
	m_sprite->SetOffset(glm::vec2(0.5f));

	m_tileLayers.reserve(4);

	// tilemap
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
	//m_sprite->SetPosition(mousePosition);
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
		_renderer->DrawQuad({ 1280.0f, 0.0f, 0.0f }, m_application->GetWindow().GetSize(), m_texture);
		_renderer->DrawQuad({ 100.0f, 100.0f, 0.0f }, { 100.0f, 100.0f }, m_playerTexture);
		//_renderer->DrawSprite(*m_sprite);
	}
	_renderer->EndScene();
	m_framebuffer->Unbind();

	m_renderer2D->BeginScene(m_postFXProjection);
	{
		if (m_enableLight == false)
		{
			m_renderer2D->DrawQuad({ 0.0f, 0.0f, 0.0f }, m_application->GetWindow().GetSize(), m_framebuffer->GetTextureAttachment());
			m_renderer2D->EndScene();
			return;
		}

		auto& lightShader = m_lightData.GetShader();
		lightShader->Bind();
		lightShader->SetMat4("u_viewProjection", m_postFXProjection);
		lightShader->SetMat4("u_sceneVP", m_camera->GetViewProjectionMatrix());
		lightShader->SetFloat3("u_ambientLight", m_ambiantLightColor);

		m_renderer2D->DrawQuad({ 0.0f, 0.0f, 0.0f }, m_application->GetWindow().GetSize(), m_framebuffer->GetTextureAttachment(), lightShader);
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