#pragma once

#include "Sapphire.h"

class TilemapLayer
	: public sph::Layer
{
public:
	TilemapLayer();
	~TilemapLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(sph::DeltaTime _dt) override;
	virtual void OnRender(const sph::Ref<sph::Renderer>& _renderer) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(sph::Event& _event) override;

	void SetCamera(const sph::Ref<sph::OrthographicCamera>& _camera) { m_camera = _camera; }

private:
	sph::Ref<sph::OrthographicCamera> m_camera = nullptr;
	sph::Scope<sph::TilemapRenderer> m_tilemapRenderer = nullptr;
	sph::Ref<sph::TileMap> m_tileMap = nullptr;
};