#ifndef SPH_APPLICATION_H
#define SPH_APPLICATION_H

#include "Sapphire/Core/Core.h"
#include "Sapphire/Layers/LayerStack.h"
#include "Sapphire/Profiling/FrameTimeData.h"

namespace sph
{
	class Window;
	class ImGuiLayer;
	class Shader;
	class VertexArray;
	class WindowCloseEvent;
	class WindowResizeEvent;

	namespace Profiling
	{
		class FrameTimeData;
	}

	class  Application
	{
	public:
		Application();
		virtual ~Application();

		virtual void Run();

		virtual void PushLayer(Layer* _layer);
		virtual void PushOverlay(Layer* _overlay);

		virtual void PopLayer(Layer* _layer);
		virtual void PopOverlay(Layer* _overlay);

		inline Window& GetWindow() { return *m_window; }

		void SetRenderer(Ref<Renderer> _renderer) { m_renderer = _renderer; }
	protected:
		virtual void Init();
		virtual void OnEvent(Event& _event);
		virtual void OnUpdate() {}
		virtual void OnRender(const Ref<Renderer>& _renderer) {}
		virtual void OnImGuiRender() {};

		bool OnWindowClosed(sph::WindowCloseEvent& _event);
		bool OnWindowResize(sph::WindowResizeEvent& _event);

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