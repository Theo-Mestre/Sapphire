#include "sphpch.h"
#include "Application.h"

#include "Sapphire/Platform/Windows/WinWindow.h"

namespace sph
{
	Application::Application()
		: m_layerStack()
		, m_window(nullptr)
		, m_isRunning(true)
	{
	}

	Application::~Application()
	{
		delete m_window;
	}

	void Application::Run()
	{
		while (m_isRunning)
		{
			OnUpdate();
			OnRender();
		}
	}

	void Application::PushLayer(Layer* _layer)
	{
		m_layerStack.PushLayer(_layer);
	}

	void Application::PushOverlay(Layer* _overlay)
	{
		m_layerStack.PushOverlay(_overlay);
	}

	void Application::PopLayer(Layer* _layer)
	{
		m_layerStack.PopLayer(_layer);
	}

	void Application::PopOverlay(Layer* _overlay)
	{
		m_layerStack.PopOverlay(_overlay);
	}

	void Application::Init()
	{
		m_window = Window::Create();

		m_window->SetEventCallback(BIND_EVENT_METHOD(Application::OnEvent));
	}

	void Application::OnEvent(sph::Event& _event)
	{
		if (_event.GetEventType() == EventType::WindowClose)
		{
			m_isRunning = false;
		}
	}

	void Application::OnUpdate()
	{
		m_window->OnUpdate();

		for (Layer* layer : m_layerStack)
		{
			layer->OnUpdate();
		}
	}

	void Application::OnRender()
	{
	}
}