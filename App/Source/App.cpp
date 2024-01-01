#include "Sapphire.h"

#include "App.h"
#include "DefaultLayer.h"

Sandbox::Sandbox()
	: sph::Application()
{
	Init();
}

void Sandbox::Init()
{
	PushLayer(new DefaultLayer());
}

void Sandbox::OnEvent(sph::Event& _event)
{
	sph::EventDispatcher dispatcher(_event);
	dispatcher.Dispatch<sph::WindowCloseEvent>([&](sph::WindowCloseEvent& _event) { return m_isRunning = false; });

	for (auto it = m_layerStack.end(); it != m_layerStack.begin();)
	{
		(*--it)->OnEvent(_event);
	}

	if (_event.GetEventType() == sph::EventType::KeyPressed)
	{
		sph::KeyPressedEvent& e = (sph::KeyPressedEvent&)_event;
		if (e.GetKeyCode() == sph::KeyCode::A)
		{
			LogInfo("Tab key is pressed (event)!");
		}
	}
}

void Sandbox::OnUpdate()
{
	sph::Application::OnUpdate();

	auto [x, y] = sph::Input::GetMousePosition();
	//LogDebug("{} - {}", x, y);
}

sph::Application* sph::CreateApplication()
{
	return new Sandbox();
}