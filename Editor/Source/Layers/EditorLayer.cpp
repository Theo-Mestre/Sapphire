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
		m_cameraController = sph::CreateScope<sph::OrthographicCameraController>(m_camera, true);

		// Renderer
		sph::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });

		m_framebuffer = sph::Framebuffer::Create({ 1280, 720 });

		m_texture = sph::Texture2D::Create("Player.png");
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
		m_framebuffer->Bind();
		m_framebuffer->Clear();
		_renderer->BeginScene(*m_camera);
		{
			_renderer->DrawQuad({ 0.0f, 0.0f, 0.0f }, { 100.0f, 100.0f }, m_texture);
		}
		_renderer->EndScene();
		m_framebuffer->Unbind();

		//_renderer->BeginScene(*m_camera);
		//{
		//	_renderer->DrawQuad({ 0.0f, 0.0f, 0.0f }, { 100.0f, 100.0f }, m_texture);
		//}
		//_renderer->EndScene();
	}

	void EditorLayer::OnImGuiRender()
	{
		if (m_enableDocking)
		{
			static bool dockspaceOpen = true;
			static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
			// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
			// because it would be confusing to have two docking targets within each others.
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
			// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
			if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
				window_flags |= ImGuiWindowFlags_NoBackground;
			// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
			// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
			// all active windows docked into it will lose their parent and become undocked.
			// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
			// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

			ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);

			ImGui::PopStyleVar();
			ImGui::PopStyleVar(2);
			// DockSpace
			ImGuiIO& io = ImGui::GetIO();
			if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
			{
				ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
				ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
			}
			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					if (ImGui::MenuItem("Exit"))
					{
						m_application->Close();
					}
					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}

			// Viewport
			OnRenderViewport();

			ImGui::End();
		}
	}

	void EditorLayer::OnEvent(sph::Event& _event)
	{
		m_cameraController->OnEvent(_event);

		EventDispatcher dispatcher(_event);
		dispatcher.Dispatch<KeyPressedEvent>([&](KeyPressedEvent& _event)->bool
			{
				if (_event.GetKeyCode() == KeyCode::U)
				{
					EnableDocking(!m_enableDocking);
				}
				return false;
			});
	}

	void EditorLayer::OnRenderViewport()
	{
		ImGui::Begin("Viewport");
		{
			auto viewportSize = ImGui::GetContentRegionAvail();
			if (m_viewportSize != *((glm::ivec2*)&viewportSize))
			{
				m_viewportSize.x = viewportSize.y;
				m_viewportSize.y = viewportSize.x;

				LogWarn("Viewport Size: {0}, {1}", m_viewportSize.x, m_viewportSize.y);
				m_cameraController->SetCameraProjection(-m_viewportSize.x / 2.0f, m_viewportSize.x / 2.0f, -m_viewportSize.y / 2.0f, m_viewportSize.y / 2.0f);
				m_framebuffer->Resize((uint32_t)m_viewportSize.x, (uint32_t)m_viewportSize.y);
			}

			uint32_t textureID = m_framebuffer->GetTextureAttachment()->GetRendererID();
			ImGui::Image((void*)textureID, viewportSize, ImVec2(0, 1), ImVec2(1, 0));
		}
		ImGui::End();
	}
}