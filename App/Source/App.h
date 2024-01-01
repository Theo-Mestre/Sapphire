#pragma once

#include "Sapphire/Application.h"

namespace sph
{
	class Window;
}

class Sandbox
	: public sph::Application
{
public:
	Sandbox();
	virtual ~Sandbox() = default;

private:
	virtual void Init() override;
	virtual void OnEvent(sph::Event& _event) override;
	virtual void OnUpdate() override;
};
