#include "TestRenderer.h"

TestRenderer::TestRenderer()
	: Layer("TestRenderer")
{
}

void TestRenderer::OnAttach()
{
	m_cameraController = sph::CreateScope<sph::OrthographicCameraController>(1280.0f / 720.0f, true);

	m_texture = sph::Texture2D::Create("Sapphire.png");
}

void TestRenderer::OnDetach()
{
}

void TestRenderer::OnUpdate(sph::DeltaTime _dt)
{
	m_rotation += _dt * 45;

	m_cameraController->OnUpdate(_dt);

	sph::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	sph::RenderCommand::Clear();

	{
		sph::Renderer2D::BeginScene(m_cameraController->GetCamera());
		
		{
			sph::Renderer2D::DrawQuad({ -1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, m_color);
			sph::Renderer2D::DrawQuad({ 0.8f, 0.2f, 0.0f }, { 0.2f, 1.5f }, m_rotation, glm::vec4(0.8f, -0.2f, 0.3f, 1.0f));
			sph::Renderer2D::DrawQuad({ -1.5f, 0.9f, 0.0f }, { 0.1f, 0.1f }, glm::vec4(0.8f, -0.2f, 0.3f, 1.0f));
			sph::Renderer2D::DrawQuad({ 1.5f, 0.9f, 0.0f }, { 1.0f, 1.0f }, m_texture);
		}
		
		glm::vec2 quadCoords = { -10.0f, -10.0f };
		glm::vec2 quadSize = { 0.1f, 0.1f };
		int32_t quadCount = 250;
		
		for (int32_t y = -quadCount; y < quadCount; y++)
		{
			for (int32_t x = -quadCount; x < quadCount; x++)
			{
				quadCoords += glm::vec2(0.1f, 0.0f);
		
				sph::Renderer2D::DrawQuad(quadCoords, quadSize, m_texture);
			}
			quadCoords += glm::vec2(0.0f, 0.1f);
			quadCoords.x = -10.0f;
		}
		
		sph::Renderer2D::EndScene();
	}
}

void TestRenderer::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Color", glm::value_ptr(m_color));

	ImGui::Begin("Stats");
	std::string statsText = std::string("frame time : " + std::to_string(sph::Time::DeltaTime) +
		"\nfps : " + std::to_string(int(1 / sph::Time::DeltaTime)) +
		"\nquad number : " + std::to_string(sph::Renderer2D::Stats::QuadCount) +
		"\ndraw call : " + std::to_string(sph::Renderer2D::Stats::DrawCalls));
	ImGui::Text(statsText.c_str());
	ImGui::End();

	ImGui::End();
}

void TestRenderer::OnEvent(sph::Event& _event)
{
	m_cameraController->OnEvent(_event);
}
