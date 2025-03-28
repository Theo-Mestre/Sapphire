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

#ifdef SPH_PLATFORM_WINDOWS
	#include "Sapphire/Platform/Windows/WindowsFileIO.h"
#endif

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
			Time::Update();

			if (m_minimized == false)
			{
				OnUpdate();

				// Update layers
				for (Layer* layer : m_layerStack)
				{
					layer->OnUpdate(Time::DeltaTime);
				}
			}
#ifndef DIST // Disable ImGui in Distribution build
			m_imGuiLayer->Begin();
			OnImGuiRender();
			for (Layer* layer : m_layerStack)
			{
				layer->OnImGuiRender();
			}
			m_imGuiLayer->End();
#endif

			// Update window
			m_window->OnUpdate();

			if (m_minimized == false)
			{
				Renderer::Stats::Reset();
				RenderCommand::Clear();

				OnRender(m_renderer);

				for (Layer* layer : m_layerStack)
				{
					layer->OnRender(m_renderer);
				}
			}
		}
	}

	void Application::Close()
	{
		m_isRunning = false;
	}

	void Application::PushLayer(Layer* _layer)
	{
		SPH_PROFILE_FUNCTION();

		_layer->SetApplication(this);
		m_layerStack.PushLayer(_layer);
	}

	void Application::PushOverlay(Layer* _overlay)
	{
		SPH_PROFILE_FUNCTION();

		_overlay->SetApplication(this);
		m_layerStack.PushOverlay(_overlay);
	}

	void Application::PopLayer(Layer* _layer)
	{
		SPH_PROFILE_FUNCTION();

		m_layerStack.PopLayer(_layer);
	}

	void Application::PopOverlay(Layer* _overlay)
	{
		SPH_PROFILE_FUNCTION();

		m_layerStack.PopOverlay(_overlay);
	}

	template<typename T>
	T* Application::GetLayerOfType()
	{
		static_assert(std::is_base_of<Layer, T>::value, "T must be a layer!");
		for (auto layer : m_layerStack)
		{
			if (typeid(*layer) == typeid(T))
			{
				return static_cast<T*>(layer);
			}
		}
	}

	template<>
	ImGuiLayer* Application::GetLayerOfType<ImGuiLayer>()
	{
		return m_imGuiLayer;
	}

	void Application::Init()
	{
		// Initialize window
		WindowProperties props;
		props.Title = "Sapphire Engine";
		props.Width = 1280;
		props.Height = 720;
		m_window = Window::Create(props);
		m_window->SetEventCallback(BIND_EVENT_METHOD(Application::OnEvent));

#ifdef SPH_PLATFORM_WINDOWS
		WindowsFileIO::SetHWND((void*)m_window->GetNativeWindow());
#endif
		// Initialize Input
		Input::Init(m_window);

		// Create a default renderer if none is set by user
		if (m_renderer == nullptr) m_renderer = CreateRef<Renderer2D>();
		m_renderer->Init();

#ifndef DIST // Disable ImGui in Distribution build
		m_imGuiLayer = new ImGuiLayer(*m_window);
		PushOverlay(m_imGuiLayer);
#endif

#ifdef SPH_VISUAL_PROFILING_ENABLED
		PushOverlay(new ProfilingLayer());
#endif
	}

	void Application::OnEvent(Event& _event)
	{
		SPH_PROFILE_FUNCTION();

		EventDispatcher dispatcher(_event);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_METHOD(Application::OnWindowClosed));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_METHOD(Application::OnWindowResize));

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
		SPH_PROFILE_FUNCTION();

		m_isRunning = false;
		return true;
	}

	bool Application::OnWindowResize(sph::WindowResizeEvent& _event)
	{
		SPH_PROFILE_FUNCTION();

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