#pragma once
#include "Sapphire/Layers/Layer.h"

#include <glm/glm.hpp>

namespace sph
{
	class Framebuffer;
}

class DefaultLayer
	: public sph::Layer
{
public:
	DefaultLayer();
	~DefaultLayer() = default;

	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate(sph::DeltaTime _dt) override;
	void OnEvent(sph::Event& _event) override;
	void OnImGuiRender() override;

	void SetColors(glm::vec4* _color1, glm::vec4* _color2)
	{
		m_color1 = _color1;
		m_color2 = _color2;
	}

private:
	glm::vec4* m_color1;
	glm::vec4* m_color2;
};