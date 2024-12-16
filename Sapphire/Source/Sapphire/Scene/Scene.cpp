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
		Camera* camera = nullptr;
		glm::mat4* cameraTransform;

		{
			auto view = m_registry.view<CameraComponent, TransformComponent>();
			
			for (auto entity : view)
			{
				auto [cameraComponent, transform] = view.get<CameraComponent, TransformComponent>(entity);
				camera = &cameraComponent.Camera;
				cameraTransform = &transform.Transform;
				break;
			}
		}

		if (!camera)
		{
			LogError("No camera entity found!");
			return;
		}

		_renderer->BeginScene(*camera, *cameraTransform);

		auto view = m_registry.view<TransformComponent, SpriteRendererComponent>();
		for (auto entity : view)
		{
			auto [transform, sprite] = view.get<TransformComponent, SpriteRendererComponent>(entity);
			_renderer->DrawQuad({0.0f, 0.0f ,0.0f }, glm::vec2(100.0f, 100.0f), 0.0f, sprite.Color);
		}

		_renderer->EndScene();
	}

	Entity Scene::CreateEntity(const std::string& _name)
	{
		Entity entity(m_registry.create(), this);
		entity.AddComponent<TransformComponent>();
		entity.AddComponent<TagComponent>(_name.empty() ? "Entity" : _name);
		return entity;
	}
}