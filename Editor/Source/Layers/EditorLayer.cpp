#include "EditorLayer.h"

namespace sph
{
	EditorLayer::EditorLayer()
		: Layer("TestLighting")
	{
	}

	EditorLayer::~EditorLayer()
	{
	}

	void EditorLayer::OnAttach()
	{
		// Camera
		ASSERT(m_camera != nullptr, "Camera is not set!");
		m_cameraController = sph::CreateScope<sph::OrthographicCameraController>(m_camera);

		// Renderer
		sph::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	}

	void EditorLayer::OnDetach()
	{
	}

	void EditorLayer::OnUpdate(sph::DeltaTime _dt)
	{
		m_cameraController->OnUpdate(_dt);
	}

	void EditorLayer::OnRender(const sph::Ref<sph::Renderer>& _renderer)
	{
		_renderer->BeginScene(*m_camera);
		{
		}
		_renderer->EndScene();
	}

	void EditorLayer::OnImGuiRender()
	{
	}

	void EditorLayer::OnEvent(sph::Event& _event)
	{
		m_cameraController->OnEvent(_event);
	}
}