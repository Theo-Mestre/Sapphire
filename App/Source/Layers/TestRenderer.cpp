#include "TestRenderer.h"

TestRenderer::TestRenderer()
	: Layer("TestRenderer")
{
}

void TestRenderer::OnAttach()
{
	m_cameraController = sph::CreateScope<sph::OrthographicCameraController>(1280.0f / 720.0f, true);

	m_texture = sph::Texture2D::Create("TileSheet.png");
	m_subTexture = sph::SubTexture2D::Create(m_texture, { 4, 6 }, { 16, 16 });

	// Set clear color
	sph::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
}

void TestRenderer::OnDetach()
{
}

void TestRenderer::OnUpdate(sph::DeltaTime _dt)
{
	m_cameraController->OnUpdate(_dt);

	sph::Renderer2D::s_stats.Reset();
	sph::RenderCommand::Clear();
	sph::Renderer2D::BeginScene(m_cameraController->GetCamera());
	{
		{
			sph::Renderer2D::DrawQuad({ 0.0f,0.0f, 0.0f }, { 1.0f, 1.0f }, m_subTexture);
		}
	}
	sph::Renderer2D::EndScene();
}

void TestRenderer::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Color", glm::value_ptr(m_color));

	ImGui::Begin("Stats");
	std::string statsText("frame time : " + std::to_string(sph::Time::DeltaTime) +
		"\nfps : " + std::to_string(int(1 / sph::Time::DeltaTime)) +
		"\nquad number : " + std::to_string(sph::Renderer2D::s_stats.QuadCount) +
		"\ndraw call : " + std::to_string(sph::Renderer2D::s_stats.DrawCalls));
	ImGui::Text(statsText.c_str());
	ImGui::End();

	ImGui::End();
}

void TestRenderer::OnEvent(sph::Event& _event)
{
	m_cameraController->OnEvent(_event);
}
