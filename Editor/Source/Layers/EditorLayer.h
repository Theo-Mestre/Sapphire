#ifndef EDITOR_LAYER_H
#define EDITOR_LAYER_H

#include "Sapphire.h"

namespace sph
{
	class ImGuiLayer;
	class SceneHierarchyPanel;
	class PropertiesPanel;
	class EditorCamera;

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
		void OpenScene(const std::string& _scenePath);
		void SaveSceneAs();

	private:
		void OnMenuBarRender();
		void OnRenderViewport();
		void OnDrawGuizmos();

		bool OnKeyPressed(KeyPressedEvent& _event);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& _event);
	private:
		Ref<Scene> m_currentScene = nullptr;

		Ref<EditorCamera> m_editorCamera = nullptr;

		Ref<UniformBuffer> m_appDataUniformBuffer = nullptr;
		Ref<Framebuffer> m_framebuffer = nullptr;

		Ref<SceneHierarchyPanel> m_hierarchyPanel;
		Ref<PropertiesPanel> m_propertiesPanel;

		int32_t m_gizmoType = -1;

		Entity m_hoveredEntity = Entity();

		Ref<Texture2D> m_texture = nullptr;

		Ref<Sprite> m_sprite = nullptr;

		bool m_enableDocking = true;
		glm::ivec2 m_viewportSize = { 0, 0 };
		glm::vec2 m_viewportBounds[2] = { {0.0f, 0.0f}, {0.0f, 0.0f} };

		ImGuiLayer* m_imguiLayer = nullptr;
	};
}
#endif