#include "imgui_internal.h"

#include "Sapphire/ImGui/ImGuiLayer.h"
#include "Sapphire/Scene/Entity.h"
#include "Sapphire/Scene/SceneSerializer.h"
#include "Sapphire/Scene/Components.h"
#include "Sapphire/Utilities/FileIO.h"
#include "Sapphire/Maths/Maths.h"
#include "Sapphire/Renderer/EditorCamera.h"

#include "EditorLayer.h"

#include "Panels/SceneHierarchyPanel.h"
#include "Panels/PropertiesPanel.h"
#include "Panels/ContentDrawerPanel.h"

namespace sph
{
	extern const std::filesystem::path g_AssetPath;

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
		RenderCommand::SetClearColor({ 0.06f, 0.06f, 0.06f, 1.0f });

		InitializeIcons();

		// ImGuiLayer
		m_imguiLayer = m_application->GetLayerOfType<ImGuiLayer>();
		ASSERT(m_imguiLayer != nullptr, "EditorLayer requires ImGuiLayer!");

		// Viewport Framebuffer
		FramebufferSpecification spec;
		spec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
		spec.Width = 1280;
		spec.Height = 720;
		m_framebuffer = Framebuffer::Create(spec);

		// Scene
		m_currentScene = CreateRef<Scene>();

		// Editor Camera
		m_editorCamera = CreateRef<EditorCamera>(30.0f, 16.0f / 9.0f, 0.1f, 1000.0f);

		// Hierarchy
		m_hierarchyPanel = CreateRef<SceneHierarchyPanel>(m_currentScene);

		// Properties
		m_propertiesPanel = CreateRef<PropertiesPanel>(m_hierarchyPanel);

		// Content Drawer
		m_contentDrawerPanel = CreateRef<ContentDrawerPanel>();
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

		switch (m_sceneState)
		{
		case SceneState::Edit:
			m_editorCamera->OnUpdate(_dt);
			m_currentScene->OnUpdateEditor(_dt);
			break;

		case SceneState::Play:
			m_currentScene->OnUpdateRuntime(_dt);
			break;
		}

		OnMousePickingUpdate();
	}

	void EditorLayer::OnRender(const Ref<Renderer>& _renderer)
	{
		SPH_PROFILE_FUNCTION();

		m_framebuffer->Bind();
		m_framebuffer->Clear();

		ASSERT(m_currentScene != nullptr, "Scene is not set!");

		switch (m_sceneState)
		{
		case SceneState::Edit:
			m_currentScene->OnRenderEditor(_renderer, m_editorCamera);
			break;

		case SceneState::Play:
			m_currentScene->OnRenderRuntime(_renderer);
			break;
		}

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
		OnSelectionToolRender();
		m_hierarchyPanel->OnImGuiRender();
		m_propertiesPanel->OnImGuiRender();
		m_contentDrawerPanel->OnImGuiRender();

		ImGui::Begin("Debug");
		{
			static bool showDemoWindow = false;
			ImGui::Checkbox("showDemoWindow", &showDemoWindow);

			if (showDemoWindow)
				ImGui::ShowDemoWindow();


			std::string name = "None";
			if (m_hoveredEntity.IsValid())
			{
				name = m_hoveredEntity.GetComponent<TagComponent>().Tag;
			}
			ImGui::Text("Hovered Entity: %s", name.c_str());

		}
		ImGui::End();

		// Viewport
		OnRenderViewport();

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_DRAWER_ITEM"))
			{
				const wchar_t* path = (const wchar_t*)payload->Data;
				OpenScene(std::filesystem::path(g_AssetPath) / path);
			}
			ImGui::EndDragDropTarget();
		}

		ImGui::End();
	}

	void EditorLayer::OnEvent(Event& _event)
	{
		SPH_PROFILE_FUNCTION();
		m_editorCamera->OnEvent(_event);

		EventDispatcher dispatcher(_event);
		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_METHOD(EditorLayer::OnKeyPressed));
		dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_METHOD(EditorLayer::OnMouseButtonPressed));
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& _event)
	{
		if (_event.GetRepeatCount() > 0) return false;
		if (Input::IsKeyPressed(KeyCode::LeftControl) == false &&
			Input::IsKeyPressed(KeyCode::RightControl) == false) return false;

		// File IO
		switch (_event.GetKeyCode())
		{
		case KeyCode::N: NewScene(); return true;
		case KeyCode::O: OpenScene(); return true;
		case KeyCode::S: SaveSceneAs(); return true;
		default: break;
		}

		// Gizmos
		if (ImGuizmo::IsUsing()) return false;
		switch (_event.GetKeyCode())
		{
		case KeyCode::Q: m_gizmoType = -1; return true;
		case KeyCode::W: m_gizmoType = ImGuizmo::OPERATION::TRANSLATE; return true;
		case KeyCode::E: m_gizmoType = ImGuizmo::OPERATION::ROTATE; return true;
		case KeyCode::R: m_gizmoType = ImGuizmo::OPERATION::SCALE; return true;
		default: return false;
		}
	}

	bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& _event)
	{
		auto mousePosition = Input::GetMousePosition();

		if (_event.GetMouseButton() == Mouse::Button::Left && ImGuizmo::IsOver() == false)
		{
			if (m_hoveredEntity.IsValid())
			{
				m_hierarchyPanel->SetSelectedEntity(m_hoveredEntity);
			}
			return true;
		}
		return false;
	}

	void EditorLayer::OnMousePickingUpdate()
	{
		m_framebuffer->Bind();
		auto pos = ImGui::GetMousePos();
		pos.x -= m_viewportBounds[0].x;
		pos.y -= m_viewportBounds[0].y;
		glm::vec2 viewportSize = m_viewportBounds[1] - m_viewportBounds[0];
		pos.y += viewportSize.y;
		pos.y = viewportSize.y - pos.y;
		int mouseX = (int)pos.x;
		int mouseY = (int)pos.y;

		if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
		{
			int32_t pixelData = m_framebuffer->ReadPixel(1, mouseX, mouseY);
			m_hoveredEntity = pixelData == -1 ? Entity() : Entity((entt::entity)pixelData, m_currentScene.get());
		}
		m_framebuffer->ClearAttachment(1, -1);
		m_framebuffer->Unbind();
	}

	void EditorLayer::OnMenuBarRender()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Save Scene", "Ctrl+S"))
				{
					SaveSceneAs();
				}
				if (ImGui::MenuItem("Open Scene", "Ctrl+O"))
				{
					OpenScene();
				}
				if (ImGui::MenuItem("New Scene", "Ctrl+N"))	
				{
					NewScene();
				}
				if (ImGui::MenuItem("Exit", "Alt+F4"))
				{
					m_application->Close();
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

			auto viewportOffset = ImGui::GetCursorPos();
			auto windowSize = ImGui::GetWindowSize();
			ImVec2 minBound = ImGui::GetWindowPos();
			minBound.x += viewportOffset.x;
			minBound.y += viewportOffset.y;

			ImVec2 maxBound = { minBound.x + windowSize.x, minBound.y + windowSize.y };
			m_viewportBounds[0] = { minBound.x, minBound.y };
			m_viewportBounds[1] = { maxBound.x, maxBound.y };

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
		float snapValue = 0.5f;

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

	void EditorLayer::OnToolbarRender()
	{
		static const float padding = 4.0f;
		float size = ImGui::GetFrameHeight() - padding * 2.0f;

		// Color
		auto& colors = ImGui::GetStyle().Colors;
		const auto& buttonHovered = colors[ImGuiCol_ButtonHovered];
		const auto& buttonActive = colors[ImGuiCol_ButtonActive];

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));

		ImTextureID icon = m_sceneState == SceneState::Edit
			? (ImTextureID)(uint64_t)m_icons["Play"]->GetRendererID()
			: (ImTextureID)(uint64_t)m_icons["Stop"]->GetRendererID();

		ImGui::SetCursorPosX(ImGui::GetWindowWidth() * 0.5f - size);
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() - padding);

		if (ImGui::ImageButton(icon, ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), padding))
		{
			m_sceneState == SceneState::Edit
				? OnScenePlay()
				: OnSceneStop();
		}
		ImGui::PopStyleColor(3);
	}

	void EditorLayer::OnSelectionToolRender()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0);
		ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImGuiLayer::BackgroundColor);

		if (ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_MenuBar))
		{
			if (ImGui::BeginMenuBar())
			{
				OnToolbarRender();

				ImGui::EndMenuBar();
			}

			// Toolbar
			constexpr int toolbarItems = 4;
			float padding = 6.0f;
			float fontSize = (ImGui::CalcTextSize("A").y - padding) * 2.0f;

			ImVec2 selectablesSize = { fontSize, fontSize };
			ImVec2 toolbarPos = { ImGui::GetWindowPos().x + ImGui::GetCursorPos().x + padding, ImGui::GetWindowPos().y + ImGui::GetCursorPos().y + padding };
			ImVec2 toolbarSize = { fontSize + padding * 2.0f , (padding + fontSize) * toolbarItems + padding };

			ImGui::SetNextWindowPos(toolbarPos);
			ImGui::SetNextWindowSize(toolbarSize);

			ImGuiWindowFlags toolbarFlags = ImGuiWindowFlags_NoDecoration
				| ImGuiWindowFlags_NoMove
				| ImGuiWindowFlags_NoScrollWithMouse
				| ImGuiWindowFlags_NoSavedSettings
				| ImGuiWindowFlags_NoBringToFrontOnFocus;

			ImGuiSelectableFlags selectableFlags = ImGuiSelectableFlags_NoPadWithHalfSpacing;

			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0.0f, padding });
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 0.0f, 0.0f });
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { padding, padding });
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 3.0f);

			void* imageRendererID[toolbarItems] =
			{ 
				(void*)m_icons["Neutral"]->GetRendererID(),
				(void*)m_icons["Translate"]->GetRendererID(),
				(void*)m_icons["Rotate"]->GetRendererID(),
				(void*)m_icons["Scale"]->GetRendererID()
			};

			int32_t guizmoType[toolbarItems] =
			{
				-1,
				ImGuizmo::OPERATION::TRANSLATE,
				ImGuizmo::OPERATION::ROTATE,
				ImGuizmo::OPERATION::SCALE
			};

			if (ImGui::Begin("##ViewportToolbar", nullptr, toolbarFlags))
			{
				// Set the window to the front
				ImGui::BringWindowToDisplayFront(ImGui::GetCurrentWindow());

				ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.5f, 0.5f));

				for (int i = 0; i < toolbarItems; i++)
				{
					ImGui::PushID(i);

					if (ImGui::Selectable("##", m_gizmoType == guizmoType[i], selectableFlags, selectablesSize))
						m_gizmoType = guizmoType[i];

					ImGui::SameLine();
					ImGui::SetCursorPosX(ImGui::GetCursorPosX() - selectablesSize.x);
					ImGui::Image(imageRendererID[i], selectablesSize);

					ImGui::PopID();
				}

				ImGui::PopStyleVar();
			}
			ImGui::PopStyleVar(4);

			ImGui::End();
		}
		ImGui::End();

		ImGui::PopStyleVar();
		ImGui::PopStyleColor();
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

	void EditorLayer::OpenScene(const std::filesystem::path& _path)
	{
		if (FileIO::GetFileExtension(_path.string()) != SPH_SCENE_FILE_EXTENSION)
		{
			LogError("SceneSerializer: Filepath is not a scene file");
			return;
		}

		m_currentScene = CreateRef<Scene>();
		m_currentScene->OnViewportResize((uint32_t)m_viewportSize.x, (uint32_t)m_viewportSize.y);
		m_hierarchyPanel->SetContext(m_currentScene);

		SceneSerializer serializer(m_currentScene);
		serializer.Deserialize(_path.string());
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

	void EditorLayer::OnScenePlay()
	{
		m_sceneState = SceneState::Play;
	}

	void EditorLayer::OnSceneStop()
	{
		m_sceneState = SceneState::Edit;
	}

	void EditorLayer::InitializeIcons()
	{
		Texture2D::Properties props;
		props.flipVertically = 0;

		m_icons["Play"] = Texture2D::Create("Editor/Icons/PlayIcon.png", props);
		m_icons["Stop"] = Texture2D::Create("Editor/Icons/StopIcon.png", props);

		// Guizmo Icons
		m_icons["Neutral"] = Texture2D::Create("Editor/Icons/NeutralIcon.png", props);
		m_icons["Translate"] = Texture2D::Create("Editor/Icons/TranslateIcon.png", props);
		m_icons["Rotate"] = Texture2D::Create("Editor/Icons/RotateIcon.png", props);
		m_icons["Scale"] = Texture2D::Create("Editor/Icons/ScaleIcon.png", props);
	}
}