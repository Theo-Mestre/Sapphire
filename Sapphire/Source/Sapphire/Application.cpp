#include "sphpch.h"

#include <glad/glad.h>

#include "Application.h"
#include "Sapphire/Input.h"
#include "Sapphire/Platform/Windows/WinWindow.h"
#include "Sapphire/ImGui/ImGuiLayer.h"

namespace sph
{
	Application::Application()
		: m_layerStack()
		, m_window(nullptr)
		, m_isRunning(true)
		, m_imGuiLayer(nullptr)
	{
		Init();
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
		sph::Logger::Init();
		LogInfo("Hello Client!");

		m_window = Window::Create();

		m_window->SetEventCallback(BIND_EVENT_METHOD(Application::OnEvent));

		// Initialize ImGui layer
		m_imGuiLayer = new sph::ImGuiLayer(*m_window);
		m_layerStack.PushOverlay(m_imGuiLayer);

		Input::Init(m_window);
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
		glClearColor(1, 0, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		for (sph::Layer* layer : m_layerStack)
		{
			layer->OnUpdate();
		}

		m_imGuiLayer->Begin();
		for (sph::Layer* layer : m_layerStack)
		{
			layer->OnImGuiRender();
		}
		m_imGuiLayer->End();

		m_window->OnUpdate();
	}

	void Application::OnRender()
	{
	}
}