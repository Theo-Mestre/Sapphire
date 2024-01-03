#include "Sapphire/Log.h"

#include "DefaultLayer.h"
#include "imgui.h"

DefaultLayer::DefaultLayer()
	: Layer("DefaultLayer")
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
}
