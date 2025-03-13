#ifndef EDITOR_LAYER_H
#define EDITOR_LAYER_H

#include "Sapphire.h"

namespace sph
{
	class ImGuiLayer;
	class EditorCamera;
	class SceneHierarchyPanel;
	class ContentDrawerPanel;
	class PropertiesPanel;

	class EditorLayer
		: public Layer
	{
	public:
		EditorLayer();
		~EditorLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(DeltaTime _dt) override;
		virtual void OnRender(const Ref<Renderer>& _renderer) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& _event) override;

		void EnableDocking(bool _enable) { m_enableDocking = _enable; }
		bool IsDockingEnabled() const { return m_enableDocking; }

		// File IO
		void NewScene();
		void OpenScene();
		void OpenScene(const std::filesystem::path& _path);
		void SaveSceneAs();

		void OnScenePlay();
		void OnSceneStop();
	private:
		void OnMousePickingUpdate();

		void OnMenuBarRender();
		void OnRenderViewport();
		void OnDrawGuizmos();
		void OnToolbarRender();

		bool OnKeyPressed(KeyPressedEvent& _event);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& _event);

	private:
		enum class SceneState
		{
			Edit = 0, Play = 1
		};

	private:
		Ref<Scene> m_currentScene = nullptr;

		Ref<EditorCamera> m_editorCamera = nullptr;

		Ref<UniformBuffer> m_appDataUniformBuffer = nullptr;
		Ref<Framebuffer> m_framebuffer = nullptr;

		Ref<SceneHierarchyPanel> m_hierarchyPanel = nullptr;
		Ref<PropertiesPanel> m_propertiesPanel = nullptr;
		Ref<ContentDrawerPanel> m_contentDrawerPanel = nullptr;

		Ref<Texture2D> m_iconPlay;
		Ref<Texture2D> m_iconStop;

		int32_t m_gizmoType = -1;

		Entity m_hoveredEntity = Entity();

		SceneState m_sceneState = SceneState::Edit;

		bool m_enableDocking = true;
		glm::ivec2 m_viewportSize = { 0, 0 };
		glm::vec2 m_viewportBounds[2] = { {0.0f, 0.0f}, {0.0f, 0.0f} };

		ImGuiLayer* m_imguiLayer = nullptr;
	};
}
#endif