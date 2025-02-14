#ifndef SPH_COMPONENTS_H
#define SPH_COMPONENTS_H

#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Sapphire/Core/Core.h"
#include "Sapphire/Scene/SceneCamera.h"
#include "Sapphire/Scene/ScriptableEntity.h"

namespace sph
{
	struct TagComponent
	{
		std::string Tag;
		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& _tag)
			: Tag(_tag)
		{
		}
	};

	struct TransformComponent
	{
		glm::mat4 Transform;

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::mat4& _transform)
			: Transform(_transform)
		{
		}

		operator glm::mat4& () { return Transform; }
		operator const glm::mat4& () const { return Transform; }
	};

	struct SpriteRendererComponent
	{
		glm::vec4 Color = { 1.0f, 1.0f, 1.0f, 1.0f };

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& _color)
			: Color(_color)
		{
		}

		operator glm::vec4& () { return Color; }
		operator const glm::vec4& () const { return Color; }
	};

	struct CameraComponent
	{
		SceneCamera Camera;
		bool IsPrimary = true;
		bool IsFixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;

		operator sph::Camera& () { return Camera; }
		operator const sph::Camera& () const { return Camera; }
	};

	struct NativeScriptComponent
	{
		ScriptableEntity* Instance = nullptr;

		std::function<void()> InstantiateFunction;
		std::function<void()> DestroyInstanceFunction;

		std::function<void(ScriptableEntity*)> OnCreateFunction;
		std::function<void(ScriptableEntity*)> OnDestroyFunction;
		std::function<void(ScriptableEntity*, DeltaTime)> OnUpdateFunction;

		template<typename T>
		void Bind()
		{
			InstantiateFunction = [&]() { Instance = new T(); };
			DestroyInstanceFunction = [&]() { delete (T*)Instance; Instance = nullptr; };

			OnCreateFunction = [](ScriptableEntity* _instance) { static_cast<T*>(_instance)->OnCreate(); };
			OnDestroyFunction = [](ScriptableEntity* _instance) { static_cast<T*>(_instance)->OnDestroy(); };
			OnUpdateFunction = [](ScriptableEntity* _instance, DeltaTime _dt) { static_cast<T*>(_instance)->OnUpdate(_dt); };
		}
	};
}
#endif