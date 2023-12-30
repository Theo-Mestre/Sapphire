#pragma once

#include "Sapphire/Application.h"
#include "DefaultLayer.h"

namespace sph
{
	class Window;
}

class Sandbox
	: public sph::Application
{
public:
	Sandbox();
	~Sandbox() = default;

private:
	void Init() override;
	void OnEvent(sph::Event& _event) override;
	void OnUpdate() override;
};
