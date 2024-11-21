#include "sphpch.h"

#include "Application.h"
#include "Sapphire/Core/Log.h"
#include "Sapphire/Core/Time.h"
#include "Sapphire/Core/Input.h"
#include "Sapphire/ImGui/ImGuiLayer.h"
#include "Sapphire/Renderer/Renderering2D.h"
#include "Sapphire/Renderer/RenderCommand.h"
#include "Sapphire/Events/ApplicationEvent.h"
#include "Sapphire/Events/KeyEvent.h"
#include "Sapphire/Platform/Windows/WinWindow.h"
#include "Sapphire/Layers/ProfilingLayer.h"
#include "Sapphire/Profiling/Profiler.h"

#include "KeyCode.h"

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
	}

	Application::~Application()
	{
		m_layerStack.Clear();

		m_renderer->Shutdown();

		delete m_window;
	}

	void Application::Run()
	{
		Init();

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
				sph::Renderer::Stats::Reset();
				sph::RenderCommand::Clear();

				OnRender(m_renderer);

				for (sph::Layer* layer : m_layerStack)
				{
					layer->OnRender(m_renderer);
				}
			}

			PROFILE_END_FRAME();
		}
	}

	void Application::Close()
	{
		m_isRunning = false;
	}

	void Application::PushLayer(Layer* _layer)
	{
		_layer->SetApplication(this);
		m_layerStack.PushLayer(_layer);
	}

	void Application::PushOverlay(Layer* _overlay)
	{
		_overlay->SetApplication(this);
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
		PROFILE_FUNCTION();

		// Initialize window
		WindowProperties props;
		props.Title = "Sapphire Engine";
		props.Width = 1280;
		props.Height = 720;
		m_window = Window::Create(props);
		m_window->SetEventCallback(BIND_EVENT_METHOD(Application::OnEvent));

		// Initialize Input
		Input::Init(m_window);

		// Create a default renderer if none is set by user
		if (!m_renderer) m_renderer = CreateRef<Renderer2D>();
		m_renderer->Init();

#ifndef DIST // Disable ImGui in Distribution build
		m_imGuiLayer = new sph::ImGuiLayer(*m_window);
		PushOverlay(m_imGuiLayer);
#endif

#ifdef SPH_VISUAL_PROFILING_ENABLED
		PushOverlay(new ProfilingLayer());
#endif
	}

	void Application::OnEvent(Event& _event)
	{
		PROFILE_FUNCTION();
		sph::EventDispatcher dispatcher(_event);
		dispatcher.Dispatch<sph::WindowCloseEvent>(BIND_EVENT_METHOD(Application::OnWindowClosed));
		dispatcher.Dispatch<sph::WindowResizeEvent>(BIND_EVENT_METHOD(Application::OnWindowResize));

		// Debug switch to enable/disable docking
		dispatcher.Dispatch<KeyPressedEvent>([&](KeyPressedEvent& _event)->bool
			{
				if (_event.GetKeyCode() == KeyCode::U)
				{
					m_imGuiLayer->EnableDocking(!m_imGuiLayer->IsDockingEnabled());
				}
				return false;
			});

		for (auto it = m_layerStack.end(); it != m_layerStack.begin();)
		{
			(*--it)->OnEvent(_event);
			if (_event.Handled)
			{
				break;
			}
		}
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

		m_renderer->OnWindowResize(_event.GetWidth(), _event.GetHeight());

		return false;
	}
}