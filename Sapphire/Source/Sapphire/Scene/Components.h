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
#include "Sapphire/Core/KeyCode.h"
#include "Sapphire/Core/Input.h"

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
		glm::vec4 TexCoords = { 0.0f, 0.0f, 1.0f, 1.0f };
		float TilingFactor = 1.0f;

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

	struct SpriteAnimatorComponent
	{
		SpriteRendererComponent* SpriteRenderer = nullptr;
		glm::i32vec2 FrameCount = { 1, 1 };
		glm::i32vec2 CurrentFrame = { 0, 0 };

		float FrameTime = 0.1f;
		float FrameTimer = 0.0f;
		bool IsLooping = true;

		SpriteAnimatorComponent() = default;
		SpriteAnimatorComponent(const SpriteAnimatorComponent&) = default;

		void Update(DeltaTime _dt)
		{
			FrameTimer += _dt;
			if (FrameTimer < FrameTime) return;

			FrameTimer = 0.0f;
			CurrentFrame.x++;
			if (CurrentFrame.x < FrameCount.x) return;

			CurrentFrame.x = IsLooping ? 0 : FrameCount.x - 1;
		};

		void ApplyTexCoords()
		{
			if (SpriteRenderer == nullptr) return;

			glm::vec2 size = { 1.0f / FrameCount.x, 1.0f / FrameCount.y };

			glm::vec2 min = { size.x * CurrentFrame.x, size.y * CurrentFrame.y };
			glm::vec2 max = { min.x + size.x, min.y + size.y };

			SpriteRenderer->TexCoords = { min, max };
		}
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

	struct ParallaxComponent 
	{
		glm::vec2 InitialPosition = { 0.0f, 0.0f };
		glm::vec2 Position = { 0.0f, 0.0f };
		glm::vec2 Speed = { 0.0f, 0.0f };
		glm::vec2 Threshold = { 0.0f, 0.0f };

		ParallaxComponent() = default;
		ParallaxComponent(const ParallaxComponent&) = default;
		~ParallaxComponent() = default;
	};

	struct PlayerController
	{
	public:
		entt::registry* m_registry = nullptr;

		TransformComponent* m_transform = nullptr;
		TransformComponent* m_camera = nullptr;
		SpriteAnimatorComponent* m_animator = nullptr;

		glm::vec2 m_cameraLimits = { 10.0f, -4.0f };
		glm::vec2 m_playerLimits = { 10.0f, -6.0f };

		float m_speed = 1.50f;

		void Update(sph::DeltaTime _dt)
		{
			float direction = 0.0f;

			if (m_transform == nullptr || m_camera == nullptr || m_animator == nullptr)
			{
				m_transform = m_registry->try_get<TransformComponent>(m_registry->view<PlayerController>().front());
				m_animator = m_registry->try_get<SpriteAnimatorComponent>(m_registry->view<PlayerController>().front());
				m_animator->CurrentFrame.y = 9;

				auto view = m_registry->view<TransformComponent, CameraComponent>();
				for (auto entity : view)
				{
					auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);
					if (camera.IsPrimary)
					{
						m_camera = &transform;
						break;
					}
				}
			}

			m_animator->CurrentFrame.y = 0;
			if (sph::Input::IsKeyPressed(sph::KeyCode::D))
			{
				m_animator->CurrentFrame.y = 2;
				direction += 1.0f;
			}
			else if (sph::Input::IsKeyPressed(sph::KeyCode::A))
			{
				m_animator->CurrentFrame.y = 1;
				direction -= 1.0f;
			}

			m_transform->Translation.x += direction * m_speed * _dt;

			if (m_transform->Translation.x < m_playerLimits.x)
			{
				m_transform->Translation.x = m_playerLimits.x;
			}
			else if (m_transform->Translation.x > m_playerLimits.y)
			{
				m_transform->Translation.x = m_playerLimits.y;
			}


			if (m_camera)
			{
				auto& cameraTranslation = m_camera->Translation;

				cameraTranslation.x = m_transform->Translation.x;

				if (cameraTranslation.x < m_cameraLimits.x)
				{
					cameraTranslation.x = m_cameraLimits.x;
				}
				else if (cameraTranslation.x > m_cameraLimits.y)
				{
					cameraTranslation.x = m_cameraLimits.y;
				}
			}
		}
	};
}
#endif