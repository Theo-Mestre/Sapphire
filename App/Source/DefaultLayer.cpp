#include "Sapphire/Log.h"

#include "DefaultLayer.h"
#include "imgui.h"

DefaultLayer::DefaultLayer()
	: Layer("DefaultLayer")
	, m_color1(nullptr)
	, m_color2(nullptr)
{
	LogInfo("DefaultLayer created");
}

void DefaultLayer::OnAttach()
{
	LogInfo("DefaultLayer attached");
}

void DefaultLayer::OnDetach()
{
	LogInfo("DefaultLayer detached");
}

void DefaultLayer::OnUpdate(sph::DeltaTime _dt)
{

}

void DefaultLayer::OnEvent(sph::Event& _event)
{
	//LogInfo(_event.ToString());
}

void DefaultLayer::OnImGuiRender()
{
	if (m_color1 == nullptr ||
		m_color2 == nullptr) return;

	ImGui::Begin("Color Settings");
	ImGui::Text("Choose first Color : ");
	ImGui::ColorEdit4("Color 1", glm::value_ptr(*m_color1));
	ImGui::Text("Choose second Color : ");
	ImGui::ColorEdit4("Color 2", glm::value_ptr(*m_color2));
	ImGui::End();

	// Dockspace
	static bool p_open = true;
	static bool opt_fullscreen_persistant = true;
	static ImGuiDockNodeFlags opt_flags = ImGuiDockNodeFlags_None;
	bool opt_fullscreen = opt_fullscreen_persistant;
		
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	
}
