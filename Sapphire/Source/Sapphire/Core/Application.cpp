#include "sphpch.h"

#include "Application.h"
#include "Sapphire/Core/Time.h"
#include "Sapphire/Core/Input.h"
#include "Sapphire/ImGui/ImGuiLayer.h"
#include "Sapphire/Renderer/Renderer.h"
#include "Sapphire/Events/ApplicationEvent.h"
#include "Sapphire/Platform/Windows/WinWindow.h"
#include "Sapphire/Layers/ProfilingLayer.h"
#include "Sapphire/Profiling/Profiler.h"

namespace sph
{
	Application::Application()
		: m_layerStack()
		, m_imGuiLayer(nullptr)
		, m_window(nullptr)
		, m_isRunning(true)
		, m_minimized(false)
	{

		sph::Logger::Init();

		m_window = Window::Create();
		m_window->SetEventCallback(BIND_EVENT_METHOD(Application::OnEvent));

		Renderer::Init();

#ifndef DIST // Disable ImGui in Distribution build
		// Initialize ImGui layer
		m_imGuiLayer = new sph::ImGuiLayer(*m_window);
		m_layerStack.PushOverlay(m_imGuiLayer);
#endif

#ifdef SPH_VISUAL_PROFILING_ENABLED
			m_layerStack.PushOverlay(new ProfilingLayer());
#endif

		Input::Init(m_window);
	}

	Application::~Application()
	{
		delete m_window;
	}

	void Application::Run()
	{
		while (m_isRunning)
		{
			PROFILE_BEGIN_FRAME();

			Time::Update();

			if (m_minimized == false)
			{
				OnUpdate();

				// Update layers
				for (sph::Layer* layer : m_layerStack)
				{
					layer->OnUpdate(Time::DeltaTime);
				}
			}
#ifndef DIST // Disable ImGui in Distribution build
			m_imGuiLayer->Begin();
			OnImGuiRender();
			for (sph::Layer* layer : m_layerStack)
			{
				layer->OnImGuiRender();
			}
			m_imGuiLayer->End();
#endif

			// Update window
			m_window->OnUpdate();

			if (m_minimized == false)
			{
				OnRender();
			}

			PROFILE_END_FRAME();
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

	void Application::OnEvent(Event& _event)
	{
		sph::EventDispatcher dispatcher(_event);
		dispatcher.Dispatch<sph::WindowCloseEvent>(BIND_EVENT_METHOD(Application::OnWindowClosed));
		dispatcher.Dispatch<sph::WindowResizeEvent>(BIND_EVENT_METHOD(Application::OnWindowResize));

		for (auto it = m_layerStack.end(); it != m_layerStack.begin();)
		{
			(*--it)->OnEvent(_event);
			//if (_event.Handled)
			//{
			//	break;
			//}
		}
	}

	void Application::OnImGuiRender()
	{

	}

	bool Application::OnWindowClosed(sph::WindowCloseEvent& _event)
	{
		m_isRunning = false;
		return true;
	}

	bool Application::OnWindowResize(sph::WindowResizeEvent& _event)
	{
		if (_event.GetWidth() == 0 || _event.GetHeight() == 0)
		{
			m_minimized = true;
			return false;
		}

		m_minimized = false;

		Renderer::OnWindowResize(_event.GetWidth(), _event.GetHeight());

		return false;
	}
}