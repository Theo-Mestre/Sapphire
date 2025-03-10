#include "sphpch.h"

#include <glfw/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "EditorCamera.h"

#include "Sapphire/Core/Input.h"
#include "Sapphire/Core/KeyCode.h"
#include "Sapphire/Core/MouseButton.h"

namespace sph
{
	EditorCamera::EditorCamera(float _fov, float _aspectRatio, float _nearClip, float _farClip)
		: m_FOV(_fov)
		, m_aspectRatio(_aspectRatio)
		, m_nearClip(_nearClip)
		, m_farClip(_farClip)
	{
	}

	void EditorCamera::OnUpdate(DeltaTime dt)
	{
		if (Input::IsKeyPressed(KeyCode::LeftAlt))
		{
			const glm::vec2& mouse = Input::GetMousePosition();
			glm::vec2 _delta = (mouse - m_initialMousePosition) * 0.003f;
			m_initialMousePosition = mouse;

			if (Input::IsMouseButtonPressed(Mouse::Middle))
			{
				MousePan(_delta);
			}
			else if (Input::IsMouseButtonPressed(Mouse::Left))
			{
				MouseRotate(_delta);
			}
			else if (Input::IsMouseButtonPressed(Mouse::Right))
			{
				MouseZoom(_delta.y);
			}
		}
		m_position = CalculatePosition();
		UpdateView();
	}

	void EditorCamera::OnEvent(Event& _event)
	{
		EventDispatcher dispatcher(_event);
		dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_METHOD(EditorCamera::OnMouseScrolled));
	}

	void EditorCamera::ResetTransform()
	{
		m_position = { 0.0f, 0.0f, 0.0f };
		m_focalPoint = { 0.0f, 0.0f, 0.0f };
		m_distance = 10.0f;
		m_pitch = 0.0f;
		m_yaw = 0.0f;

		UpdateView();
	}

	glm::vec3 EditorCamera::GetUpDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	glm::vec3 EditorCamera::GetRightDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
	}

	glm::vec3 EditorCamera::GetForwardDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
	}

	glm::quat EditorCamera::GetOrientation() const
	{
		return glm::quat(glm::vec3(-m_pitch, -m_yaw, 0.0f));
	}

	void EditorCamera::UpdateProjection()
	{
		m_aspectRatio = m_viewportWidth / m_viewportHeight;
		m_projection = glm::perspective(glm::radians(m_FOV), m_aspectRatio, m_nearClip, m_farClip);
	}

	void EditorCamera::UpdateView()
	{
		m_viewMatrix = glm::lookAt(m_position, m_position + GetForwardDirection(), GetUpDirection());
	}

	bool EditorCamera::OnMouseScrolled(MouseScrolledEvent& _event)
	{
		MouseZoom(_event.GetYOffset() * 0.1f);
		UpdateView();
		return false;
	}

	void EditorCamera::MousePan(const glm::vec2& _delta)
	{
		auto [xSpeed, ySpeed] = PanSpeed();
		m_focalPoint += -GetRightDirection() * _delta.x * xSpeed * m_distance;
		m_focalPoint += GetUpDirection() * _delta.y * ySpeed * m_distance;
	}

	void EditorCamera::MouseRotate(const glm::vec2& _delta)
	{
		float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
		m_yaw += yawSign * _delta.x * RotationSpeed();
		m_pitch += _delta.y * RotationSpeed();
	}

	void EditorCamera::MouseZoom(float _delta)
	{
		m_distance -= _delta * ZoomSpeed();
		if (m_distance < 1.0f)
		{
			m_focalPoint += GetForwardDirection();
			m_distance = 1.0f;
		}
	}

	glm::vec3 EditorCamera::CalculatePosition() const
	{
		return m_focalPoint - GetForwardDirection() * m_distance;
	}

	std::pair<float, float> EditorCamera::PanSpeed() const
	{
		float x = std::min(m_viewportWidth / 1000.0f, 2.4f);
		float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

		float y = std::min(m_viewportHeight / 1000.0f, 2.4f);
		float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

		return { xFactor, yFactor };
	}

	float EditorCamera::RotationSpeed() const
	{
		return 0.8f;
	}

	float EditorCamera::ZoomSpeed() const
	{
		float distance = m_distance * 0.2f;
		distance = std::max(distance, 0.0f);
		float speed = distance * distance;
		speed = std::min(speed, 100.0f);
		return speed;
	}
}