#include "Sapphire/Log.h"

#include "DefaultLayer.h"

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
	LogInfo(_event.ToString());
}
