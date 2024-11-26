#include "TestLayer.h"

TestLayer::TestLayer()
	: Layer("TestLighting")
{
}

TestLayer::~TestLayer()
{
}

void TestLayer::OnAttach()
{
	// Camera
	ASSERT(m_camera != nullptr, "Camera is not set!");
	m_cameraController = sph::CreateScope<sph::OrthographicCameraController>(m_camera);

	// Renderer
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

	// Sprite
	auto texture = sph::Texture2D::Create("Sprite.png");
	m_sprite = sph::Sprite::Create(texture, true);
	m_sprite->SetSize({ 500.0f, 500.0f });
	m_sprite->SetOffset(glm::vec2(0.5f));
}

void TestLayer::OnDetach()
{
}

void TestLayer::OnUpdate(sph::DeltaTime _dt)
{
	m_cameraController->OnUpdate(_dt);
}

void TestLayer::OnRender(const sph::Ref<sph::Renderer>& _renderer)
{
	_renderer->BeginScene(*m_camera);
	{
		_renderer->DrawQuad({ 1280.0f, 0.0f, 0.0f }, m_application->GetWindow().GetSize(), m_texture);
		_renderer->DrawSprite(*m_sprite);
		_renderer->DrawQuad({ 100.0f, 100.0f, 0.0f }, { 100.0f, 100.0f }, m_playerTexture);
	}
	_renderer->EndScene();
}

void TestLayer::OnImGuiRender()
{
}

void TestLayer::OnEvent(sph::Event& _event)
{
	m_cameraController->OnEvent(_event);
}