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
		: Layer("EditorLayer")
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

		// ImGuiLayer
		m_imguiLayer = m_application->GetLayerOfType<ImGuiLayer>();
		ASSERT(m_imguiLayer != nullptr, "EditorLayer requires ImGuiLayer!");

		// Viewport Framebuffer
		FramebufferSpecification spec;
		spec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
		spec.Width = 1280;
		spec.Height = 720;

		m_framebuffer = Framebuffer::Create(spec);
		m_framebuffer->Clear();

		// Scene
		m_currentScene = CreateRef<Scene>();

		// Editor Camera
		m_editorCamera = CreateRef<EditorCamera>(30.0f, 16.0f / 9.0f, 0.1f, 1000.0f);

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

		const FramebufferSpecification& spec = m_framebuffer->GetSpecification();
		bool viewportSizeIsValid = m_viewportSize.x > 0.0f && m_viewportSize.y > 0.0f;
		bool viewportSizeChanged = spec.Width != m_viewportSize.x || spec.Height != m_viewportSize.y;

		if (viewportSizeIsValid && viewportSizeChanged)
		{
			m_framebuffer->Resize((uint32_t)m_viewportSize.x, (uint32_t)m_viewportSize.y);
			m_editorCamera->SetViewportSize((float)m_viewportSize.x, (float)m_viewportSize.y);
			m_currentScene->OnViewportResize((uint32_t)m_viewportSize.x, (uint32_t)m_viewportSize.y);
		}

		m_editorCamera->OnUpdate(_dt);

		m_framebuffer->ClearAttachment(1, -1);

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
		default:
		}

		// Gizmos
		if (ImGuizmo::IsUsing()) return;
		switch (_event.GetKeyCode())
		{
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

			uint64_t textureID = (uint64_t)m_framebuffer->GetColorAttachmentRendererID();
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
			OpenScene(*filepath);
		}
	}

	void EditorLayer::OpenScene(const std::string& _scenePath)
	{
		m_currentScene = CreateRef<Scene>();
		m_currentScene->OnViewportResize((uint32_t)m_viewportSize.x, (uint32_t)m_viewportSize.y);
		m_hierarchyPanel->SetContext(m_currentScene);

		SceneSerializer serializer(m_currentScene);
		serializer.Deserialize(_scenePath);
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