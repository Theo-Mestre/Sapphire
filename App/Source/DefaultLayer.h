#pragma once
#include "Sapphire/Layers/Layer.h"

class DefaultLayer
	: public sph::Layer
{
public:
	DefaultLayer();
	~DefaultLayer() = default;

	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate() override;
	void OnEvent(sph::Event& _event) override;
	void OnImGuiRender() override;
};