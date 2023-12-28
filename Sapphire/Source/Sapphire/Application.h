#ifndef CORE_APPLICATION_H
#define CORE_APPLICATION_H

#include "Sapphire/Core.h"
#include "Sapphire/Layers/LayerStack.h"

namespace sph
{
	class Window;

	class SPH_API Application
	{
	public:
		Application();
		virtual ~Application();

		virtual void Run();

		virtual void PushLayer(Layer* _layer);
		virtual void PushOverlay(Layer* _overlay);

		virtual void PopLayer(Layer* _layer);
		virtual void PopOverlay(Layer* _overlay);
	protected:
		virtual void Init();
		virtual void OnEvent(sph::Event& _event);
		virtual void OnUpdate();
		virtual void OnRender();

	protected:
		LayerStack m_layerStack;
		Window* m_window;
		bool m_isRunning;
	};

	Application* CreateApplication();
}
#endif