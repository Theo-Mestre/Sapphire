#include "sphpch.h"
#include "Scene.h"

#include "Sapphire/Renderer/Renderer.h"
#include "Sapphire/Scene/Components.h"
#include "Sapphire/Scene/Entity.h"

namespace sph
{
	Scene::Scene()
	{
		SPH_PROFILE_FUNCTION();

	}

	Scene::~Scene()
	{
		SPH_PROFILE_FUNCTION();

		m_registry.clear();
	}

	void Scene::OnUpdate(DeltaTime _dt)
	{
		SPH_PROFILE_FUNCTION();

		{
			m_registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
				{
					if (!nsc.Instance)
					{
						nsc.InstantiateFunction();
						nsc.Instance->m_Entity = Entity{ entity, this };

						if (nsc.OnCreateFunction)
						{
							nsc.OnCreateFunction(nsc.Instance);
						}
					}

					if (nsc.OnUpdateFunction)
					{
						nsc.OnUpdateFunction(nsc.Instance, _dt);
					}
				});
		}
	}

	void Scene::OnRender(const Ref<Renderer>& _renderer)
	{
		SPH_PROFILE_FUNCTION();

		Camera* camera = nullptr;
		glm::mat4* cameraTransform = nullptr;
		{
			auto view = m_registry.view<CameraComponent, TransformComponent>();

			for (auto entity : view)
			{
				auto [cameraComponent, transform] = view.get<CameraComponent, TransformComponent>(entity);

				if (!cameraComponent.IsPrimary) continue;

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
			_renderer->DrawQuad(transform, sprite.Color);
		}

		_renderer->EndScene();
	}

	Entity Scene::CreateEntity(const std::string& _name)
	{
		SPH_PROFILE_FUNCTION();

		Entity entity(m_registry.create(), this);
		entity.AddComponent<TransformComponent>();
		entity.AddComponent<TagComponent>(_name.empty() ? "Entity" : _name);
		return entity;
	}

	void Scene::OnViewportResize(uint32_t _width, uint32_t _height)
	{
		m_viewportWidth = _width;
		m_viewportHeight = _height;

		// Resize our non-FixedAspectRatio cameras
		auto view = m_registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			
			if (cameraComponent.IsFixedAspectRatio) continue;
			
			cameraComponent.Camera.SetViewportSize(_width, _height);
		}
	}
}