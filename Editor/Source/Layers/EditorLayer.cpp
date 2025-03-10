#include "Sapphire/ImGui/ImGuiLayer.h"
#include "Sapphire/Scene/Entity.h"
#include "Sapphire/Scene/SceneSerializer.h"
#include "Sapphire/Utilities/FileIO.h"
#include "Sapphire/Maths/Maths.h"
#include "Sapphire/Renderer/EditorCamera.h"

#include "EditorLayer.h"

#include "Panels/SceneHierarchyPanel.h"
#include "Panels/PropertiesPanel.h"

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
		SPH_PROFILE_FUNCTION();

		// Renderer
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });

		auto windowSize = m_application->GetWindow().GetSize();
		m_framebuffer = Framebuffer::Create({ (uint32_t)windowSize.x, (uint32_t)windowSize.y });

		m_texture = Texture2D::Create("Player.png");

		m_currentScene = CreateRef<Scene>();
		m_currentScene->SetName("Test Scene");

		m_editorCamera = CreateRef<EditorCamera>(30.0f, 1.778f, 0.1f, 1000.0f);

		Entity entity = Entity::Create(m_currentScene, "Chicken");
		entity.AddComponent<SpriteRendererComponent>(m_texture);

		auto& transform = entity.GetComponent<TransformComponent>();
		transform.Translation = { 0.0f, 0.0f, 0.0f };
		transform.Scale = { 1.0f, 1.0f, 1.0f };

		Entity entity2 = Entity::Create(m_currentScene, "Rect");
		entity2.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
		auto& transform2 = entity2.GetComponent<TransformComponent>();
		transform2.Translation = { 5.0f, 0.0f, 0.0f };
		transform2.Scale = { 2.0f, 1.0f, 1.0f };

		m_mainCamera = Entity::Create(m_currentScene, "Main Camera");
		m_mainCamera.GetComponent<TransformComponent>().Translation = { 0.0f, 0.0f, 0.0f };
		auto camera = m_mainCamera.AddComponent<CameraComponent>().Camera;
		camera.SetOrthographic(10.0f, -100.0f, 100.0f);

		m_secondCamera = Entity::Create(m_currentScene, "Second Camera");
		m_secondCamera.AddComponent<CameraComponent>().IsPrimary = false;
		m_secondCamera.GetComponent<TransformComponent>().Translation = { 10.0f, 10.0f, 0.0f };

		// Hierarchy
		m_hierarchyPanel = CreateRef<SceneHierarchyPanel>(m_currentScene);

		// Properties
		m_propertiesPanel = CreateRef<PropertiesPanel>(m_hierarchyPanel);
	}

	void EditorLayer::OnDetach()
	{
		SPH_PROFILE_FUNCTION();
	}

	void EditorLayer::OnUpdate(DeltaTime _dt)
	{
		SPH_PROFILE_FUNCTION();

		FramebufferSpecification spec = m_framebuffer->GetSpecification();
		if (m_viewportSize.x > 0.0f && m_viewportSize.y > 0.0f &&
			(spec.Width != m_viewportSize.x || spec.Height != m_viewportSize.y))
		{
			m_framebuffer->Resize((uint32_t)m_viewportSize.x, (uint32_t)m_viewportSize.y);
			m_editorCamera->SetViewportSize(m_viewportSize.x, m_viewportSize.y);
			m_currentScene->OnViewportResize((uint32_t)m_viewportSize.x, (uint32_t)m_viewportSize.y);
		}

		m_editorCamera->OnUpdate(_dt);

		ASSERT(m_currentScene != nullptr, "Scene is not set!");
		m_currentScene->OnUpdateEditor(_dt);
	}

	void EditorLayer::OnRender(const Ref<Renderer>& _renderer)
	{
		SPH_PROFILE_FUNCTION();

		m_framebuffer->Bind();
		m_framebuffer->Clear();

		ASSERT(m_currentScene != nullptr, "Scene is not set!");
		m_currentScene->OnRenderEditor(_renderer, m_editorCamera);

		m_framebuffer->Unbind();
	}

	void EditorLayer::OnImGuiRender()
	{
		SPH_PROFILE_FUNCTION();

		if (m_enableDocking == false) return;

		static ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;
		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar 
			| ImGuiWindowFlags_NoDocking
			| ImGuiWindowFlags_NoTitleBar
			| ImGuiWindowFlags_NoCollapse
			| ImGuiWindowFlags_NoResize
			| ImGuiWindowFlags_NoMove
			| ImGuiWindowFlags_NoBringToFrontOnFocus
			| ImGuiWindowFlags_NoNavFocus;

		if (dockspaceFlags & ImGuiDockNodeFlags_PassthruCentralNode)
		{
			windowFlags |= ImGuiWindowFlags_NoBackground;
		}

		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		ImGui::Begin("DockSpace", &m_enableDocking, windowFlags);

		ImGui::PopStyleVar(3);

		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspaceFlags);
		}

		OnMenuBarRender();

		m_hierarchyPanel->OnImGuiRender();
		m_propertiesPanel->OnImGuiRender();

		// Viewport
		OnRenderViewport();

		ImGui::End();
	}

	void EditorLayer::OnEvent(Event& _event)
	{
		SPH_PROFILE_FUNCTION();
		m_editorCamera->OnEvent(_event);

		EventDispatcher dispatcher(_event);
		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_METHOD(EditorLayer::OnKeyPressed));
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& _event)
	{
		if (_event.GetRepeatCount() > 0) return false;
		if (Input::IsKeyPressed(KeyCode::LeftControl) == false &&
			Input::IsKeyPressed(KeyCode::RightControl) == false) return false;

		switch (_event.GetKeyCode())
		{
			// File IO
		case KeyCode::N: NewScene(); return true;
		case KeyCode::O: OpenScene(); return true;
		case KeyCode::S: SaveSceneAs(); return true;

			// Gizmos
		case KeyCode::Q: m_gizmoType = -1; return true;
		case KeyCode::W: m_gizmoType = ImGuizmo::OPERATION::TRANSLATE; return true;
		case KeyCode::E: m_gizmoType = ImGuizmo::OPERATION::ROTATE; return true;
		case KeyCode::R: m_gizmoType = ImGuizmo::OPERATION::SCALE; return true;
		default: return false;
		}
	}

	void EditorLayer::OnMenuBarRender()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Exit"))
				{
					m_application->Close();
				}
				if (ImGui::MenuItem("Save Scene"))
				{
					SaveSceneAs();
				}
				if (ImGui::MenuItem("Open Scene"))
				{
					OpenScene();
				}
				if (ImGui::MenuItem("New Scene"))
				{
					NewScene();
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Editor Camera"))
			{
				if (ImGui::MenuItem("Reset Transform"))
				{
					m_editorCamera->ResetTransform();
				}

				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}
		ImGui::PopStyleVar();
	}

	void EditorLayer::OnRenderViewport()
	{
		SPH_PROFILE_FUNCTION();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

		ImGui::Begin("Viewport");
		{
			m_imguiLayer->SetViewportFocused(ImGui::IsWindowFocused());
			m_imguiLayer->SetViewportHovered(ImGui::IsWindowHovered());

			auto viewportSize = ImGui::GetContentRegionAvail();
			m_viewportSize.x = (int32_t)viewportSize.x;
			m_viewportSize.y = (int32_t)viewportSize.y;

			uint64_t textureID = (uint64_t)m_framebuffer->GetTextureAttachment()->GetRendererID();
			ImGui::Image((void*)textureID, viewportSize, ImVec2(0, 1), ImVec2(1, 0));

			OnDrawGuizmos();
		}
		ImGui::End();

		ImGui::PopStyleVar();
	}

	void EditorLayer::OnDrawGuizmos()
	{
		// Guizmos
		Entity selectedEntity = m_hierarchyPanel->GetSelectedEntity();
		if (!selectedEntity.IsValid() || m_gizmoType == -1) return;

		ImGuizmo::SetOrthographic(false);
		ImGuizmo::SetDrawlist();

		const glm::mat4& cameraProjection = m_editorCamera->GetProjection();
		glm::mat4 cameraView = m_editorCamera->GetViewMatrix();

		float windowWidth = (float)ImGui::GetWindowWidth();
		float windowHeight = (float)ImGui::GetWindowHeight();
		ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

		// Entity transform
		auto& tc = selectedEntity.GetComponent<TransformComponent>();
		glm::mat4 transform = tc.GetTransform();

		// Snapping
		bool snap = Input::IsKeyPressed(KeyCode::LeftControl);
		float snapValue = 0.5f; // Snap to 0.5m for translation/scale
		// Snap to 45 degrees for rotation
		if (m_gizmoType == ImGuizmo::OPERATION::ROTATE)
		{
			snapValue = 45.0f;
		}

		float snapValues[3] = { snapValue, snapValue, snapValue };

		ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
			(ImGuizmo::OPERATION)m_gizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
			nullptr, snap ? snapValues : nullptr);

		if (ImGuizmo::IsUsing())
		{
			glm::vec3 translation, rotation, scale;
			Maths::DecomposeTransform(transform, translation, rotation, scale);

			glm::vec3 deltaRotation = rotation - tc.Rotation;
			tc.Translation = translation;
			tc.Rotation += deltaRotation;
			tc.Scale = scale;
		}
	}

	void EditorLayer::NewScene()
	{
		m_currentScene = CreateRef<Scene>();
		m_currentScene->OnViewportResize((uint32_t)m_viewportSize.x, (uint32_t)m_viewportSize.y);
		m_hierarchyPanel->SetContext(m_currentScene);
	}

	void EditorLayer::OpenScene()
	{
		std::optional<std::string> filepath = FileIO::OpenFile("Sapphire Scene (*.sph)\0*.sph\0");
		if (filepath)
		{
			m_currentScene = CreateRef<Scene>();
			m_currentScene->OnViewportResize((uint32_t)m_viewportSize.x, (uint32_t)m_viewportSize.y);
			m_hierarchyPanel->SetContext(m_currentScene);

			SceneSerializer serializer(m_currentScene);
			serializer.Deserialize(*filepath);
		}
	}

	void EditorLayer::SaveSceneAs()
	{
		std::optional<std::string> filepath = FileIO::SaveFile("Sapphire Scene (*.sph)\0*.sph\0");
		if (filepath)
		{
			SceneSerializer serializer(m_currentScene);
			serializer.Serialize(*filepath);
		}
	}
}