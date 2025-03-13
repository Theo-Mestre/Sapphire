#include "sphpch.h"
#include "Scene.h"

#include "Sapphire/Renderer/Renderer.h"
#include "Sapphire/Renderer/EditorCamera.h"
#include "Sapphire/Scene/Components.h"
#include "Sapphire/Scene/Entity.h"
#include "Sapphire/Scene/SceneSerializer.h"

namespace sph
{
	Scene::Scene()
		: m_name("Untitled Scene")
	{ 	}

	Scene::Scene(const std::string& _name)
		: m_name(_name)
	{	}

	Scene::~Scene()
	{
		SPH_PROFILE_FUNCTION();

		m_registry.clear();
	}

	void Scene::OnUpdateRuntime(DeltaTime _dt)
	{
		SPH_PROFILE_FUNCTION();

		m_registry.view<NativeScriptComponent>().each([=](auto _entity, auto& _comp)
			{
				if (!_comp.Instance)
				{
					_comp.Instance = _comp.InstanstiateScript();
					_comp.Instance->m_Entity = Entity{ _entity, this };
					_comp.Instance->OnCreate();
				}

				_comp.Instance->OnUpdate(_dt);
			});

		m_registry.view<SpriteAnimatorComponent>().each([=](auto _entity, auto& _comp)
			{
				_comp.Update(_dt);
				_comp.Apply();
			});
	}

	void Scene::OnUpdateEditor(DeltaTime _dt)
	{
		m_registry.view<SpriteAnimatorComponent>().each([=](auto _entity, auto& _comp)
			{
				_comp.Update(_dt);
				_comp.Apply();
			});
	}

	void Scene::OnRenderRuntime(const Ref<Renderer>& _renderer)
	{
		SPH_PROFILE_FUNCTION();

		auto mainCameraEntity = GetPrimaryCameraEntity();
		if (mainCameraEntity.IsValid() == false) return;

		Camera& camera = mainCameraEntity.GetComponent<CameraComponent>().Camera;
		glm::mat4 cameraTransform = mainCameraEntity.GetComponent<TransformComponent>().GetTransform();

		_renderer->BeginScene(camera, cameraTransform);

		auto view = m_registry.view<TransformComponent, SpriteRendererComponent>();
		for (auto _entity : view)
		{
			auto [transform, sprite] = view.get<TransformComponent, SpriteRendererComponent>(_entity);
			_renderer->DrawSprite(transform.GetTransform(), sprite, (int32_t)_entity);
		}

		_renderer->EndScene();
	}

	void Scene::OnRenderEditor(const Ref<Renderer>& _renderer, const Ref<EditorCamera>& _camera)
	{
		SPH_PROFILE_FUNCTION();

		_renderer->BeginScene(_camera->GetViewProjection());
		auto group = m_registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto _entity : group)
		{
			auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(_entity);
			_renderer->DrawSprite(transform.GetTransform(), sprite, (int32_t)_entity);
		}
		_renderer->EndScene();
	}

	Entity Scene::CreateEntity(const std::string& _name)
	{
		SPH_PROFILE_FUNCTION();

		Entity _entity(m_registry.create(), this);
		_entity.AddComponent<TransformComponent>();
		_entity.AddComponent<TagComponent>(_name.empty() ? "Entity" : _name);
		return _entity;
	}

	void Scene::DestroyEntity(Entity _entity)
	{
		SPH_PROFILE_FUNCTION();
		m_registry.destroy(_entity);
	}

	void Scene::OnViewportResize(uint32_t _width, uint32_t _height)
	{
		m_viewportWidth = _width;
		m_viewportHeight = _height;

		auto view = m_registry.view<CameraComponent>();
		for (auto _entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(_entity);

			if (cameraComponent.IsFixedAspectRatio) continue;

			cameraComponent.Camera.SetViewportSize(_width, _height);
		}
	}

	Entity Scene::GetPrimaryCameraEntity()
	{
		Camera* camera = nullptr;
		{
			auto view = m_registry.view<CameraComponent>();

			for (auto _entity : view)
			{
				auto cameraComponent = view.get<CameraComponent>(_entity);

				if (!cameraComponent.IsPrimary) continue;

				return Entity{ _entity, this };
			}
		}

		return {};
	}

	template<typename T>
	void Scene::OnComponentAdded(Entity _entity, T& component)
	{
		static_assert(false && "Unknown component type!");
	}

	template<>
	void Scene::OnComponentAdded<TransformComponent>(Entity _entity, TransformComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<CameraComponent>(Entity _entity, CameraComponent& component)
	{
		component.Camera.SetViewportSize(m_viewportWidth, m_viewportHeight);
	}

	template<>
	void Scene::OnComponentAdded<SpriteRendererComponent>(Entity _entity, SpriteRendererComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<TagComponent>(Entity _entity, TagComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<NativeScriptComponent>(Entity _entity, NativeScriptComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<SpriteAnimatorComponent>(Entity _entity, SpriteAnimatorComponent& component)
	{
		if (component.SpriteRenderer == nullptr)
		{
			if (_entity.HasComponent<SpriteRendererComponent>())
			{
				component.SpriteRenderer = &_entity.GetComponent<SpriteRendererComponent>();
			}
			else
			{
				component.SpriteRenderer = &_entity.AddComponent<SpriteRendererComponent>();
			}
		}

		component.ApplyTexCoords();
	}
}