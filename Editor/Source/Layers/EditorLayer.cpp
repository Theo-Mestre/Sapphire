#include "Sapphire/ImGui/ImGuiLayer.h"
#include "Sapphire/Scene/Entity.h"

#include "EditorLayer.h"

#include "Panels/SceneHierarchyPanel.h"
#include "Panels/PropertiesPanel.h"

namespace sph
{
	// Test Script
	class CameraController : public ScriptableEntity
	{
	public:
		void OnUpdate(DeltaTime ts)
		{
			auto& translation = GetComponent<TransformComponent>().Translation;

			static const float speed = 5.0f;

			if (Input::IsKeyPressed(KeyCode::A))
				translation.x -= speed * ts;
			if (Input::IsKeyPressed(KeyCode::D))
				translation.x += speed * ts;
			if (Input::IsKeyPressed(KeyCode::W))
				translation.y += speed * ts;
			if (Input::IsKeyPressed(KeyCode::S))
				translation.y -= speed * ts;
		}
	};

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

		Entity entity = Entity::Create(m_currentScene, "Square");
		entity.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

		auto& transform = entity.GetComponent<TransformComponent>();
		transform.Translation = { 0.0f, 0.0f, 0.0f };
		transform.Scale = { 1.0f, 1.0f, 1.0f };

		Entity entity2 = Entity::Create(m_currentScene, "Rect");
		entity2.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
		auto& transform2 = entity2.GetComponent<TransformComponent>();
		transform2.Translation = { 5.0f, 0.0f, 0.0f };
		transform2.Scale = { 2.0f, 1.0f, 1.0f };

		m_mainCamera = Entity::Create(m_currentScene, "Main Camera");
		m_mainCamera.AddComponent<CameraComponent>();
		m_mainCamera.GetComponent<TransformComponent>().Translation = { 0.0f, 0.0f, 0.0f };
		m_mainCamera.AddComponent<NativeScriptComponent>().Bind<CameraController>();

		m_secondCamera = Entity::Create(m_currentScene, "Second Camera");
		m_secondCamera.AddComponent<CameraComponent>().IsPrimary = false;
		m_secondCamera.GetComponent<TransformComponent>().Translation = { 10.0f, 10.0f, 0.0f };
		m_secondCamera.AddComponent<NativeScriptComponent>().Bind<CameraController>();

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

			m_currentScene->OnViewportResize((uint32_t)m_viewportSize.x, (uint32_t)m_viewportSize.y);
		}

		ASSERT(m_currentScene != nullptr, "Scene is not set!");
		m_currentScene->OnUpdate(_dt);
	}

	void EditorLayer::OnRender(const Ref<Renderer>& _renderer)
	{
		SPH_PROFILE_FUNCTION();

		m_framebuffer->Bind();
		m_framebuffer->Clear();

		ASSERT(m_currentScene != nullptr, "Scene is not set!");
		m_currentScene->OnRender(_renderer);

		m_framebuffer->Unbind();
	}

	void EditorLayer::OnImGuiRender()
	{
		SPH_PROFILE_FUNCTION();

		if (m_enableDocking == false) return;

		static ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;
		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

		windowFlags |= ImGuiWindowFlags_NoTitleBar
			| ImGuiWindowFlags_NoCollapse
			| ImGuiWindowFlags_NoResize
			| ImGuiWindowFlags_NoMove
			| ImGuiWindowFlags_NoBringToFrontOnFocus
			| ImGuiWindowFlags_NoNavFocus;

		if (dockspaceFlags & ImGuiDockNodeFlags_PassthruCentralNode)
		{
			windowFlags |= ImGuiWindowFlags_NoBackground;
		}

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		ImGui::Begin("DockSpace", &m_enableDocking, windowFlags);

		ImGui::PopStyleVar();
		ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiStyle& style = ImGui::GetStyle();

		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspaceFlags);
		}

		ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4(0.08f, 0.08f, 0.08f, 1.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

		if (ImGui::BeginMainMenuBar())
		{

			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Exit"))
				{
					m_application->Close();
				}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();

		}
		ImGui::PopStyleColor();
		ImGui::PopStyleVar();

		m_hierarchyPanel->OnImGuiRender();
		m_propertiesPanel->OnImGuiRender();

		ImGui::ShowDemoWindow();

		// Viewport
		OnRenderViewport();

		ImGui::End();
	}

	void EditorLayer::OnEvent(Event& _event)
	{
		SPH_PROFILE_FUNCTION();
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
			m_viewportSize.x = viewportSize.x;
			m_viewportSize.y = viewportSize.y;

			uint32_t textureID = m_framebuffer->GetTextureAttachment()->GetRendererID();
			ImGui::Image((void*)textureID, viewportSize, ImVec2(0, 1), ImVec2(1, 0));
		}
		ImGui::End();

		ImGui::PopStyleVar();
	}
}