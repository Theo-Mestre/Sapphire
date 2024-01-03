#pragma once

#include "Sapphire/Application.h"

namespace sph
{
	class Window;
	class VextexArray;
	class Shader;
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
	virtual void OnRender() override;

private:
	std::shared_ptr<sph::VertexArray> m_triVA;
	std::shared_ptr<sph::VertexArray> m_rectVA;
	std::shared_ptr<sph::Shader> m_shader;
};
