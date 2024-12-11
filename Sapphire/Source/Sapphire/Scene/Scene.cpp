#include "sphpch.h"
#include "Scene.h"

#include "Sapphire/Renderer/Renderer.h"
#include "Sapphire/Scene/Components.h"
#include "Sapphire/Scene/Entity.h"

namespace sph
{
	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
		m_registry.clear();
	}

	void Scene::OnUpdate(DeltaTime _dt)
	{
	}

	void Scene::OnRender(const Ref<Renderer>& _renderer)
	{
		for (auto entity : m_registry.view<TransformComponent, SpriteRendererComponent, TagComponent>())
		{
			auto [transform, sprite] = m_registry.get<TransformComponent, SpriteRendererComponent>(entity);
			_renderer->DrawQuad(transform.Position, glm::vec2(transform.Scale), transform.Rotation.y, sprite.Color);
		}
	}

	Entity Scene::CreateEntity(const std::string& _name)
	{
		Entity entity(m_registry.create(), this);
		entity.AddComponent<TransformComponent>();
		entity.AddComponent<TagComponent>(_name.empty() ? "Entity" : _name);
		return entity;
	}
}