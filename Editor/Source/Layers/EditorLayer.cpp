#include "EditorLayer.h"
#include "Sapphire/ImGui/ImGuiLayer.h"
#include "Sapphire/Scene/Entity.h"

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

		// Camera
		ASSERT(m_camera != nullptr, "Camera is not set!");
		m_cameraController = CreateScope<OrthographicCameraController>(m_camera, true);

		// Renderer
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });

		auto windowSize = m_application->GetWindow().GetSize();
		m_framebuffer = Framebuffer::Create({ (uint32_t)windowSize.x, (uint32_t)windowSize.y });

		m_texture = Texture2D::Create("Player.png");

		m_currentScene = CreateRef<Scene>();

		Entity entity = Entity::Create(m_currentScene, "Square");
		entity.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

		auto& transform = entity.GetComponent<TransformComponent>();
		transform.Transform = glm::translate(glm::mat4(1.0f), { -300.0f, -300.0f, 0.0f }) *
			glm::scale(glm::mat4(1.0f), { 100.0f, 100.0f, 0.0f });


		m_cameraEntity = Entity::Create(m_currentScene, "Camera");
		m_cameraEntity.AddComponent<CameraComponent>(glm::ortho(-640.0f, 640.0f, -360.f, 360.f));
		auto& cameraTransform = m_cameraEntity.GetComponent<TransformComponent>();
		cameraTransform.Transform = glm::translate(glm::mat4(1.0f), { 0.0f, 0.0f, 0.0f });
	}

	void EditorLayer::OnDetach()
	{
		SPH_PROFILE_FUNCTION();
	}

	void EditorLayer::OnUpdate(DeltaTime _dt)
	{
		SPH_PROFILE_FUNCTION();

		m_cameraController->OnUpdate(_dt);

		ASSERT(m_currentScene != nullptr, "Scene is not set!");
		m_currentScene->OnUpdate(_dt);
	}

	void EditorLayer::OnRender(const Ref<Renderer>& _renderer)
	{
		SPH_PROFILE_FUNCTION();

		m_framebuffer->Bind();
		m_framebuffer->Clear();
		_renderer->BeginScene(*m_camera);
		{
			_renderer->DrawQuad({ 0.0f, 0.0f, 0.0f }, { 100.0f, 100.0f }, m_texture);

			ASSERT(m_currentScene != nullptr, "Scene is not set!");
		}
		_renderer->EndScene();

		m_currentScene->OnRender(_renderer);

		m_framebuffer->Unbind();
	}

	void EditorLayer::OnImGuiRender()
	{
		SPH_PROFILE_FUNCTION();

		if (m_enableDocking)
		{
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
				windowFlags |= ImGuiWindowFlags_NoBackground;

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

			ImGui::Begin("DockSpace Demo", &m_enableDocking, windowFlags);

			ImGui::PopStyleVar();
			ImGui::PopStyleVar(2);

			// DockSpace
			ImGuiIO& io = ImGui::GetIO();
			if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
			{
				ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
				ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspaceFlags);
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

			// Debug // Entity Editing
			ImGui::Begin("Debug");
			{
				{
					auto cameraView = m_currentScene->Registry().view<TransformComponent, CameraComponent>();
					for (auto entity : cameraView)
					{
						auto [transform, camera] = cameraView.get<TransformComponent, CameraComponent>(entity);
						ImGui::Text("Camera Entity");
						ImGui::Text("Entity ID: %d", (uint32_t)entity);
						ImGui::DragFloat3("Position", glm::value_ptr(transform.Transform[3]), 0.1f);
						ImGui::DragFloat3("Scale", glm::value_ptr(transform.Transform[2]), 0.1f);
					}
				}
				{
					auto view = m_currentScene->Registry().view<TransformComponent, SpriteRendererComponent, TagComponent>();
					for (auto entity : view)
					{
						auto [transform, sprite, tag] = view.get<TransformComponent, SpriteRendererComponent, TagComponent>(entity);
						ImGui::Text("Entity ID: %d", (uint32_t)entity);
						ImGui::Text("Entity Tag: %s", tag.Tag.c_str());
						ImGui::DragFloat3("Position", glm::value_ptr(transform.Transform[3]), 0.1f);
						ImGui::DragFloat3("Scale", glm::value_ptr(transform.Transform[2]), 0.1f);
						ImGui::ColorEdit4("Color", &sprite.Color.r);
					}
				}
			}
			ImGui::End();

			// Viewport
			OnRenderViewport();

			ImGui::End();
		}
	}

	void EditorLayer::OnEvent(Event& _event)
	{
		SPH_PROFILE_FUNCTION();

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
		SPH_PROFILE_FUNCTION();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

		ImGui::Begin("Viewport");
		{
			m_imguiLayer->SetViewportFocused(ImGui::IsWindowFocused());
			m_imguiLayer->SetViewportHovered(ImGui::IsWindowHovered());

			auto viewportSize = ImGui::GetContentRegionAvail();
			if (m_viewportSize != *((glm::ivec2*)&viewportSize)
				&& viewportSize.x > 0 && viewportSize.y > 0)
			{
				m_viewportSize.x = (float)viewportSize.x;
				m_viewportSize.y = (float)viewportSize.y;


				m_cameraController->SetCameraProjection(-m_viewportSize.x / 2.0f, m_viewportSize.x / 2.0f, -m_viewportSize.y / 2.0f, m_viewportSize.y / 2.0f);
				m_cameraEntity.GetComponent<CameraComponent>().Camera.SetProjection(glm::ortho(-m_viewportSize.x / 2.0f, m_viewportSize.x / 2.0f, -m_viewportSize.y / 2.0f, m_viewportSize.y / 2.0f));
				m_framebuffer->Resize((uint32_t)m_viewportSize.x, (uint32_t)m_viewportSize.y);
			}

			uint32_t textureID = m_framebuffer->GetTextureAttachment()->GetRendererID();
			ImGui::Image((void*)textureID, viewportSize, ImVec2(0, 1), ImVec2(1, 0));
		}
		ImGui::End();

		ImGui::PopStyleVar();
	}
}