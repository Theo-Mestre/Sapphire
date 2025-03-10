#ifndef SPH_COMPONENTS_H
#define SPH_COMPONENTS_H

#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "Sapphire/Core/Core.h"
#include "Sapphire/Scene/SceneCamera.h"
#include "Sapphire/Scene/ScriptableEntity.h"

namespace sph
{
	class Texture2D;

	struct TagComponent
	{
		std::string Tag;
		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& _tag)
			: Tag(_tag == "" ? "Default Entity" : _tag)
		{
		}
	};

	struct TransformComponent
	{
		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default; 
		TransformComponent(const glm::vec3& _translation)
			: Translation(_translation) {
		}

		glm::mat4 GetTransform() const
		{
			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

			return glm::translate(glm::mat4(1.0f), Translation)
				* rotation
				* glm::scale(glm::mat4(1.0f), Scale);
		}
	};

	struct SpriteRendererComponent
	{
		sph::Ref<sph::Texture2D> Texture = nullptr;
		glm::vec4 Color = { 1.0f, 1.0f, 1.0f, 1.0f };

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const sph::Ref<sph::Texture2D>& _texture)
			: Texture(_texture)
			, Color(1.0f, 1.0f, 1.0f, 1.0f)
		{
		}
		SpriteRendererComponent(const glm::vec4& _color)
			: Texture(nullptr)
			, Color(_color)
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

		ScriptableEntity* (*InstanstiateScript)() = nullptr;
		void (*DestroyScript)(NativeScriptComponent*) = nullptr;

		template<typename T>
		void Bind()
		{
			static_assert(std::is_base_of<ScriptableEntity, T>(), "T must derive from ScriptableEntity!");

			InstanstiateScript = []()->ScriptableEntity* { return static_cast<ScriptableEntity*>(new T()); };
			DestroyScript = [](NativeScriptComponent* _comp)
				{
					delete _comp->Instance;
					_comp->Instance = nullptr;
				};
		}
	};
}
#endif