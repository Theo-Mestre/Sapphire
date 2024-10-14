#include "glad/glad.h"

#include "TestLighting.h"

TestLighting::TestLighting(sph::Application* const _app)
	: Layer("TestLighting")
	, m_app(_app)
	, m_ambiantLightColor(0.1f, 0.1f, 0.1f)
	, m_lightData()
	, m_mousePosition(0.0f)
{
}

TestLighting::~TestLighting()
{
}

void TestLighting::OnAttach()
{
	m_cameraController = sph::CreateScope<sph::OrthographicCameraController>(1280.0f / 720.0f, true);

	m_renderer2D = sph::CreateScope<sph::Renderer2D>();
	m_renderer2D->Init();

	sph::Texture::Properties props;
	props.WrapModeS = sph::Texture::WrapMode::Repeat;
	props.WrapModeT = sph::Texture::WrapMode::Repeat;
	m_texture = sph::Texture2D::Create("Background.png", props);

	// AppData buffer
	float data[2] = { (float)m_app->GetWindow().GetWidth(), (float)m_app->GetWindow().GetHeight() };
	m_appDataUniformBuffer = sph::UniformBuffer::Create({ { sph::ShaderDataType::Float2, "Resolution"}}, 0);
	m_appDataUniformBuffer->SetData(data, sizeof(data));

	// Create Lights
	constexpr uint32_t lightCount = 1;
	std::vector<Light> lights =
	{
		{ { 1.0f, 1.0f, 1.0f , 1.0f}, { 0.0f, 0.0f }, 1.f, 0.3f },
	};
	m_lightData.UploadLightsData(lights);

	sph::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
}

void TestLighting::OnDetach()
{
	m_renderer2D->Shutdown();
}

void TestLighting::OnUpdate(sph::DeltaTime _dt)
{
	m_cameraController->OnUpdate(_dt);

	m_mousePosition = { sph::Input::GetMouseX() / (1280), 1 - sph::Input::GetMouseY() / (720) };
	m_lightData.GetUniformBuffer()->SetData(&m_mousePosition, sizeof(glm::vec2), 32);
}

void TestLighting::OnRender(const sph::Ref<sph::Renderer>& _renderer)
{
	sph::Renderer2D::s_stats.Reset();
	sph::RenderCommand::Clear();

	m_renderer2D->BeginScene(m_cameraController->GetCamera());

	auto& lightShader = m_lightData.GetShader();
	lightShader->Bind();
	lightShader->SetMat4("u_viewProjection", m_cameraController->GetCamera().GetViewProjectionMatrix());
	lightShader->SetFloat3("u_ambientLight", m_ambiantLightColor);

	m_renderer2D->DrawQuad({ 0.0f, 0.0f, 0.0f }, glm::vec2(2.0f, 1.0f), m_texture, lightShader);

	m_renderer2D->EndScene();
}

void TestLighting::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit3("Ambiant Light Color", glm::value_ptr(m_ambiantLightColor));
	ImGui::End();
}

void TestLighting::OnEvent(sph::Event& _event)
{
	m_cameraController->OnEvent(_event);
}