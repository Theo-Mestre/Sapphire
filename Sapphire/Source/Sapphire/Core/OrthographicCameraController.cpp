#include "sphpch.h"
#include "OrthographicCameraController.h"

#include "Sapphire/Core/Input.h"
#include "Sapphire/Core/KeyCode.h"
#include "Sapphire/Events/MouseEvent.h"
#include "Sapphire/Events/ApplicationEvent.h"

namespace sph
{
	OrthographicCameraController::OrthographicCameraController(const Ref<OrthographicCamera>& _camera, bool _canZoom, bool _canRotate)
		: m_zoomLevel(1.0f)
		, m_camera(_camera)
		, m_position({ 0.0f, 0.0f, 0.0f })
		, m_canZoom(_canZoom)
		, m_zoomSpeed(0.25f)
		, m_canRotate(_canRotate)
		, m_rotation(0.0f)
		, m_translationSpeed(500.0f)
		, m_rotationSpeed(180.0f)
		, m_minZoomLevel(0.25f)
		, m_maxZoomLevel(10.0f)
	{
		SPH_PROFILE_FUNCTION();

		m_camera->SetPosition(m_position);
	}

	void OrthographicCameraController::OnUpdate(DeltaTime _dt)
	{
		SPH_PROFILE_FUNCTION();

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

		if (m_canRotate)
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

			m_camera->SetRotation(0.0f, 0.0f, m_rotation);
		}
		m_camera->SetPosition(m_position);
	}

	void OrthographicCameraController::OnEvent(Event& _e)
	{
		SPH_PROFILE_FUNCTION();

		EventDispatcher dispatcher(_e);
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_METHOD(OrthographicCameraController::OnWindowResized));

		if (m_canZoom)
		{
			dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_METHOD(OrthographicCameraController::OnMouseScrolled));
		}
	}

	void OrthographicCameraController::SetCameraProjection(float _left, float _right, float _bottom, float _top)
	{
		SPH_PROFILE_FUNCTION();

		m_camera->SetProjectionMatrix(_left, _right, _bottom, _top);
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& _e)
	{
		SPH_PROFILE_FUNCTION();

		// Calculate the new zoom level
		m_zoomLevel -= _e.GetYOffset() * m_zoomSpeed;
		m_zoomLevel = std::clamp(m_zoomLevel, m_minZoomLevel, m_maxZoomLevel);
		
		m_camera->SetScale(m_zoomLevel);

		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& _e)
	{
		SPH_PROFILE_FUNCTION();

		glm::vec2 halfSizeZoomed = _e.GetSize() / 2.0f / m_zoomLevel;
		m_camera->SetProjectionMatrix(-halfSizeZoomed.x, halfSizeZoomed.x, -halfSizeZoomed.y, halfSizeZoomed.y);
		return false;
	}
}