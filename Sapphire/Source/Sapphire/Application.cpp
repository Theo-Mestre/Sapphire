#include "sphpch.h"

#include "Application.h"
#include "Sapphire/Time.h"
#include "Sapphire/Input.h"
#include "Sapphire/Platform/Windows/WinWindow.h"
#include "Sapphire/ImGui/ImGuiLayer.h"
#include "Sapphire/Renderer/VertexArray.h"
#include "Sapphire/Renderer/Shader.h"
#include "Sapphire/Renderer/Buffer.h"
#include "Sapphire/Renderer/Renderer.h"

namespace sph
{
	Application::Application()
		: m_layerStack()
		, m_window(nullptr)
		, m_isRunning(true)
		, m_imGuiLayer(nullptr)
	{
		sph::Logger::Init();

		m_window = Window::Create();
		m_window->SetEventCallback(BIND_EVENT_METHOD(Application::OnEvent));

		// Initialize ImGui layer
		m_imGuiLayer = new sph::ImGuiLayer(*m_window);
		m_layerStack.PushOverlay(m_imGuiLayer);

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
			Time::Update();

			OnUpdate();

			// Update layers
			for (sph::Layer* layer : m_layerStack)
			{
				layer->OnUpdate();
			}

			// Render ImGui
			m_imGuiLayer->Begin();
			OnImGuiRender();
			for (sph::Layer* layer : m_layerStack)
			{
				layer->OnImGuiRender();
			}
			m_imGuiLayer->End();

			// Update window
			m_window->OnUpdate();

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
}