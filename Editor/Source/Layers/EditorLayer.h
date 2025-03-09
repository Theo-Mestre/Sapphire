#ifndef EDITOR_LAYER_H
#define EDITOR_LAYER_H

#include "Sapphire.h"

namespace sph
{
	class ImGuiLayer;
	class SceneHierarchyPanel;
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

		void SetCamera(const Ref<OrthographicCamera>& _camera) { m_camera = _camera; }

		void EnableDocking(bool _enable) { m_enableDocking = _enable; }
		bool IsDockingEnabled() const { return m_enableDocking; }

		void SetImGUILayer(ImGuiLayer* _imguiLayer) { m_imguiLayer = _imguiLayer; }

		// File IO
		void NewScene();
		void OpenScene();
		void SaveSceneAs();

	private:
		void OnRenderViewport();

		bool OnKeyPressed(KeyPressedEvent& _event);
	private:
		Ref<OrthographicCamera> m_camera = nullptr;
		Scope<OrthographicCameraController> m_cameraController = nullptr;

		Ref<Scene> m_currentScene = nullptr;

		Ref<UniformBuffer> m_appDataUniformBuffer = nullptr;
		Ref<Framebuffer> m_framebuffer = nullptr;

		Ref<SceneHierarchyPanel> m_hierarchyPanel;
		Ref<PropertiesPanel> m_propertiesPanel;

		Entity m_mainCamera;
		Entity m_secondCamera;
		bool m_primaryCamera = true;

		Ref<Texture2D> m_texture = nullptr;

		Ref<Sprite> m_sprite = nullptr;

		bool m_enableDocking = true;
		glm::ivec2 m_viewportSize = { 0, 0 };

		ImGuiLayer* m_imguiLayer = nullptr;
	};
}
#endif