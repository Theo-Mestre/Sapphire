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

void DefaultLayer::OnUpdate()
{

}

void DefaultLayer::OnEvent(sph::Event& _event)
{
	//LogInfo(_event.ToString());
}

void DefaultLayer::OnImGuiRender()
{
	ImGui::Begin("Test");
	ImGui::Text("Hello World!");
	ImGui::End();

	if (m_color1 == nullptr ||
		m_color2 == nullptr) return;

	ImGui::Begin("Color Settings");
	ImGui::Text("Choose first Color : ");
	ImGui::ColorEdit4("Color 1", glm::value_ptr(*m_color1));
	ImGui::Text("Choose second Color : ");
	ImGui::ColorEdit4("Color 2", glm::value_ptr(*m_color2));
	ImGui::End();
}
