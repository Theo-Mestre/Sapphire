#ifndef SPH_APPLICATION_H
#define SPH_APPLICATION_H

#include "Sapphire/Core/Core.h"
#include "Sapphire/Layers/LayerStack.h"

namespace sph
{
	class Window;
	class ImGuiLayer;
	class Shader;
	class VertexArray;
	class WindowCloseEvent;
	class WindowResizeEvent;

	class Application
	{
	public:
		Application();
		virtual ~Application();

		virtual void Run();

		virtual void Close();

		// Layer management
		virtual void PushLayer(Layer* _layer);
		virtual void PushOverlay(Layer* _overlay);

		virtual void PopLayer(Layer* _layer);
		virtual void PopOverlay(Layer* _overlay);

		// Window and rendering
		inline Window& GetWindow() const { return *m_window; }
		void SetRenderer(Ref<Renderer> _renderer) { m_renderer = _renderer; }
		Ref<Renderer> GetRenderer() const { return m_renderer; }
	protected:
		// Application lifecycle
		virtual void Init();
		virtual void OnEvent(Event& _event);
		virtual void OnUpdate() {}
		virtual void OnRender(const Ref<Renderer>& _renderer) {}
		virtual void OnImGuiRender() {};

		// Events
		bool OnWindowClosed(WindowCloseEvent& _event);
		bool OnWindowResize(WindowResizeEvent& _event);

	protected:
		LayerStack m_layerStack;
		ImGuiLayer* m_imGuiLayer;
		Window* m_window;
		bool m_isRunning;
		bool m_minimized;

		Ref<Renderer> m_renderer;
	};

	Application* CreateApplication();
}
#endif