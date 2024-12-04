#ifndef EDITOR_LAYER_H
#define EDITOR_LAYER_H

#include "Sapphire.h"

namespace sph
{
	class ImGuiLayer;

	class EditorLayer
		: public sph::Layer
	{
	public:
		EditorLayer();
		~EditorLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(sph::DeltaTime _dt) override;
		virtual void OnRender(const sph::Ref<sph::Renderer>& _renderer) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(sph::Event& _event) override;

		void SetCamera(const sph::Ref<sph::OrthographicCamera>& _camera) { m_camera = _camera; }

		void EnableDocking(bool _enable) { m_enableDocking = _enable; }
		bool IsDockingEnabled() const { return m_enableDocking; }

		void SetImGUILayer(ImGuiLayer* _imguiLayer) { m_imguiLayer = _imguiLayer; }

	private:
		void OnRenderViewport();

	private:
		sph::Ref<sph::OrthographicCamera> m_camera = nullptr;
		sph::Scope<sph::OrthographicCameraController> m_cameraController = nullptr;

		sph::Ref<sph::UniformBuffer> m_appDataUniformBuffer = nullptr;
		sph::Ref<sph::Framebuffer> m_framebuffer = nullptr;

		sph::Ref<sph::Texture2D> m_texture = nullptr;

		sph::Ref<sph::Sprite> m_sprite = nullptr;

		bool m_enableDocking = true;
		glm::ivec2 m_viewportSize = { 0, 0 };

		ImGuiLayer* m_imguiLayer = nullptr;
	};
}
#endif