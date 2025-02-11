#ifndef SPH_COMPONENTS_H
#define SPH_COMPONENTS_H

#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Sapphire/Core/Core.h"
#include "Sapphire/Scene/SceneCamera.h"

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
}
#endif