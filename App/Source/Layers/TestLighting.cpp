#include "glad/glad.h"

#include "TestLighting.h"

TestLighting::TestLighting(sph::Application* const _app)
	: Layer("TestLighting")
	, m_app(_app)
	, m_ambiantLightColor(0.1f, 0.1f, 0.1f)
	, m_lightData()
	, m_mousePosition(100.0f, 100.0f) // Test position
	, m_framebuffer(nullptr)
	, m_playerTexture(nullptr)
{
}

TestLighting::~TestLighting()
{
}

void TestLighting::OnAttach()
{
	glm::vec2 halfSize = m_app->GetWindow().GetSize() / 2.0f;
	m_camera = sph::CreateRef<sph::OrthographicCamera>(-halfSize.x, halfSize.x, -halfSize.y, halfSize.y);

	m_renderer2D = sph::CreateScope<sph::Renderer2D>();
	m_renderer2D->Init();

	m_texture = sph::Texture2D::Create("Background.png");
	m_playerTexture = sph::Texture2D::Create("Player.png");

	m_framebuffer = sph::Framebuffer::Create({ (uint32_t)m_app->GetWindow().GetWidth(), (uint32_t)m_app->GetWindow().GetHeight() });

	// AppData buffer
	float data[2] = { (float)m_app->GetWindow().GetWidth(), (float)m_app->GetWindow().GetHeight() };
	m_appDataUniformBuffer = sph::UniformBuffer::Create({ { sph::ShaderDataType::Float2, "Resolution"} }, 0);
	m_appDataUniformBuffer->SetData(data, sizeof(data));

	// Create Lights
	constexpr uint32_t lightCount = 1;
	std::vector<Light> lights =
	{
		{ { 1.0f, 1.0f, 1.0f , 1.0f}, { 100.0f, 100.0f }, 1.f, 0.3f },
	};
	m_lightData.UploadLightsData(lights);

	sph::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
}

void TestLighting::OnDetach()
{
	m_renderer2D->Shutdown();
}

void TestLighting::OnUpdate(sph::DeltaTime _dt)
{
	//m_lightData.GetUniformBuffer()->SetData(&m_mousePosition, sizeof(glm::vec2), 32);
	glm::vec3 cameraPosition = m_camera->GetPosition();

	sph::KeyCode::KeyCode inputs[] =
	{
		sph::KeyCode::W,
		sph::KeyCode::A,
		sph::KeyCode::S,
		sph::KeyCode::D,
	};

	// move camera depending on the camera
	for (uint32_t i = 0; i < 4; i++)
	{
		if (sph::Input::IsKeyPressed(inputs[i]))
		{
			switch (inputs[i])
			{
			case sph::KeyCode::W:
				cameraPosition.y += 500 * _dt;
				break;
			case sph::KeyCode::S:
				cameraPosition.y -= 500 * _dt;
				break;
			case sph::KeyCode::A:
				cameraPosition.x -= 500 * _dt;
				break;
			case sph::KeyCode::D:
				cameraPosition.x += 500 * _dt;
				break;
			default:
				break;
			}
		}
	}
	m_camera->SetPosition(cameraPosition);
}

void TestLighting::OnRender(const sph::Ref<sph::Renderer>& _renderer)
{
	sph::Renderer2D::s_stats.Reset();
	sph::RenderCommand::Clear();

	auto& m_renderer = m_renderer2D;//std::dynamic_pointer_cast<sph::Renderer2D>(_renderer);


	m_framebuffer->Bind();
	m_framebuffer->Clear();
	m_renderer->BeginScene(*m_camera);
	{
		m_renderer->DrawQuad({ 0.0f, 0.0f, 0.0f }, m_app->GetWindow().GetSize(), m_texture);
		m_renderer->DrawQuad({ 100.0f, 100.0f, 0.0f }, { 100, 100 }, m_playerTexture);
	}
	m_renderer->EndScene();
	m_framebuffer->Unbind();


	auto halfSize = m_app->GetWindow().GetSize() / 2.0f;
	static sph::OrthographicCamera camera(-halfSize.x, halfSize.x, -halfSize.y, halfSize.y);

	m_renderer->BeginScene(camera);
	{
		auto& lightShader = m_lightData.GetShader();
		lightShader->Bind();
		lightShader->SetMat4("u_viewProjection", camera.GetViewProjectionMatrix());
		lightShader->SetMat4("u_sceneVP", m_camera->GetViewProjectionMatrix());
		lightShader->SetFloat3("u_ambientLight", m_ambiantLightColor);

		m_renderer->DrawQuad({ 0.0f, 0.0f, 0.0f }, m_app->GetWindow().GetSize(), m_framebuffer->GetTextureAttachment(), lightShader);
	}
	m_renderer->EndScene();

	//{ // Render to framebuffer
	//	m_framebuffer->Bind();
	//	m_framebuffer->AttachTexture();
	//	m_framebuffer->Clear();
	//
	//
	//	m_renderer2D->DrawQuad({ 0.0f, 0.0f, 0.0f }, glm::vec2(2.0f, 1.0f), m_tileMap);
	//
	//	m_framebuffer->Unbind();
	//}
	//
	//{ // Render framebuffer
	//	m_renderer2D->DrawQuad({ 0.0f, 0.0f, 0.0f }, glm::vec2(1.0f, .5f), m_framebuffer->GetTextureAttachment());
	//}

}

void TestLighting::OnImGuiRender()
{
}

void TestLighting::OnEvent(sph::Event& _event)
{
}