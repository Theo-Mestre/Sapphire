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
				_comp.ApplyTexCoords();
			});

		m_registry.view<ParallaxComponent>().each([=](auto _entity, auto& _comp)
			{
				auto& tc = m_registry.get<TransformComponent>(_entity);
				_comp.Position = tc.Translation;
				_comp.Position += _comp.Speed * _dt;

				if (glm::abs(_comp.Position.x - _comp.InitialPosition.x) >= glm::abs(_comp.Threshold.x))
				{
					_comp.Position.x = _comp.InitialPosition.x;
				}
				if (glm::abs(_comp.Position.y - _comp.InitialPosition.y) >= glm::abs(_comp.Threshold.y))
				{
					_comp.Position.y = _comp.InitialPosition.y;
				}

				tc.Translation = { _comp.Position.x, _comp.Position.y, tc.Translation.z };
			});

		m_registry.view<PlayerController>().each([=](auto _entity, auto& _comp)
			{
				_comp.Update(_dt);
			});
	}

	void Scene::OnUpdateEditor(DeltaTime _dt)
	{
		m_registry.view<SpriteAnimatorComponent>().each([=](auto _entity, auto& _comp)
			{
				_comp.Update(_dt);
				_comp.ApplyTexCoords();
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


#define VARIABLE_TO_STRING(var) #var
	template<typename T>
	void Scene::OnComponentAdded(Entity _entity, T& _component)
	{
		LogError("OnComponentAdded not implemented for component type: " VARIABLE_TO_STRING(T));
	}

	template<>
	void Scene::OnComponentAdded<TransformComponent>(Entity _entity, TransformComponent& _component)
	{
	}

	template<>
	void Scene::OnComponentAdded<CameraComponent>(Entity _entity, CameraComponent& _component)
	{
		_component.Camera.SetViewportSize(m_viewportWidth, m_viewportHeight);
	}

	template<>
	void Scene::OnComponentAdded<SpriteRendererComponent>(Entity _entity, SpriteRendererComponent& _component)
	{
	}

	template<>
	void Scene::OnComponentAdded<TagComponent>(Entity _entity, TagComponent& _component)
	{
	}

	template<>
	void Scene::OnComponentAdded<NativeScriptComponent>(Entity _entity, NativeScriptComponent& _component)
	{
	}

	template<>
	void Scene::OnComponentAdded<SpriteAnimatorComponent>(Entity _entity, SpriteAnimatorComponent& _component)
	{
		if (_component.SpriteRenderer == nullptr)
		{
			if (_entity.HasComponent<SpriteRendererComponent>())
			{
				_component.SpriteRenderer = &_entity.GetComponent<SpriteRendererComponent>();
			}
			else
			{
				_component.SpriteRenderer = &_entity.AddComponent<SpriteRendererComponent>();
			}
		}

		_component.ApplyTexCoords();
	}

	template<>
	void Scene::OnComponentAdded<ParallaxComponent>(Entity _entity, ParallaxComponent& _component)
	{
		auto translation = _entity.GetComponent<TransformComponent>().Translation;

		_component.Position = translation;

	}

	template<>
	void Scene::OnComponentAdded<PlayerController>(Entity _entity, PlayerController& _component)
	{
		_component.m_registry = &m_registry;
	}
}