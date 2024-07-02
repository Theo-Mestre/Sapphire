#include "sphpch.h"
#include "OrthographicCameraController.h"

#include "Sapphire/Input.h"
#include "Sapphire/KeyCode.h"
#include "Sapphire/Events/MouseEvent.h"
#include "Sapphire/Events/ApplicationEvent.h"

namespace sph
{
	OrthographicCameraController::OrthographicCameraController(float _aspectRatio, bool _m_rotate)
		: m_aspectRatio(_aspectRatio)
		, m_zoomLevel(1.0f)
		, m_camera(-_aspectRatio, _aspectRatio, -1.0f, 1.0f)
		, m_position({ 0.0f, 0.0f, 0.0f })
		, m_rotate(_m_rotate)
		, m_rotation(0.0f)
		, m_translationSpeed(1.0f)
		, m_rotationSpeed(180.0f)
		, m_zoomSpeed(0.25f)
		, m_minZoomLevel(0.25f)
		, m_maxZoomLevel(10.0f)
	{
		m_camera.SetPosition(m_position);

		SetCameraProjection(-m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel);
	}

	void OrthographicCameraController::OnUpdate(DeltaTime _dt)
	{
		if (Input::IsKeyPressed(KeyCode::A))
		{
			m_position.x -= cos(glm::radians(m_rotation)) * m_translationSpeed * _dt;
			m_position.y -= sin(glm::radians(m_rotation)) * m_translationSpeed * _dt;
		}
		else if (Input::IsKeyPressed(KeyCode::D))
		{
			m_position.x += cos(glm::radians(m_rotation)) * m_translationSpeed * _dt;
			m_position.y += sin(glm::radians(m_rotation)) * m_translationSpeed * _dt;
		}

		if (Input::IsKeyPressed(KeyCode::W))
		{
			m_position.x += -sin(glm::radians(m_rotation)) * m_translationSpeed * _dt;
			m_position.y += cos(glm::radians(m_rotation)) * m_translationSpeed * _dt;
		}
		else if (Input::IsKeyPressed(KeyCode::S))
		{
			m_position.x -= -sin(glm::radians(m_rotation)) * m_translationSpeed * _dt;
			m_position.y -= cos(glm::radians(m_rotation)) * m_translationSpeed * _dt;
		}

		if (m_rotate)
		{
			if (Input::IsKeyPressed(KeyCode::Q))
			{
				m_rotation -= m_rotationSpeed * _dt;
			}
			else if (Input::IsKeyPressed(KeyCode::E))
			{
				m_rotation += m_rotationSpeed * _dt;
			}

			if (m_rotation > 180.0f) m_rotation -= 360.0f;
			else if (m_rotation <= -180.0f) m_rotation += 360.0f;

			m_camera.SetRotation(0.0f, 0.0f, m_rotation);
		}
		m_camera.SetPosition(m_position);
	}

	void OrthographicCameraController::OnEvent(Event& _e)
	{
		EventDispatcher dispatcher(_e);
		dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_METHOD(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_METHOD(OrthographicCameraController::OnWindowResized));
	}

	void OrthographicCameraController::SetAspectRatio(float aspectRatio)
	{
		m_aspectRatio = aspectRatio;
		SetCameraProjection(-m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel);
	}

	void OrthographicCameraController::SetCameraProjection(float _left, float _right, float _bottom, float _top)
	{
		m_camera.SetProjectionMatrix(_left, _right, _bottom, _top);
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& _e)
	{
		m_zoomLevel -= _e.GetYOffset() * m_zoomSpeed;
		m_zoomLevel = std::clamp(m_zoomLevel, m_minZoomLevel, m_maxZoomLevel);
		SetCameraProjection(-m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel);

		m_translationSpeed = m_zoomLevel;

		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& _e)
	{
		return false;
	}
}