#ifndef SAPPHIRE_ORTHOGRAPHIC_CAMERA_CONTROLLER_H
#define SAPPHIRE_ORTHOGRAPHIC_CAMERA_CONTROLLER_H

#include "Sapphire/Renderer/Camera.h"
#include "Sapphire/Core/Time.h"

namespace sph
{
	class Event;
	class MouseScrolledEvent;
	class WindowResizeEvent;

	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(const Ref<OrthographicCamera>& _camera, bool _canZoom = false, bool _canRotate = false);

		void OnUpdate(DeltaTime _dt);
		void OnEvent(Event& _e);

		inline Ref<OrthographicCamera>& GetCamera() { return m_camera; }
		inline const Ref<OrthographicCamera>& GetCamera() const { return m_camera; }

		inline float GetZoomLevel() const { return m_zoomLevel; }
		inline void SetZoomLevel(float _level) { m_zoomLevel = _level; }

		inline void SetCameraPosition(const glm::vec3& _position) { m_position = _position; m_camera->SetPosition(_position); }
		inline const glm::vec3& GetCameraPosition() const { return m_position; }

		inline void SetCameraRotation(float _rotation) { m_rotation = _rotation; m_camera->SetRotation(0.0f, 0.0f, _rotation); }
		inline float GetCameraRotation() const { return m_rotation; }

		inline void SetCameraTranslationSpeed(float _speed) { m_translationSpeed = _speed; }
		inline void SetCameraRotationSpeed(float _speed) { m_rotationSpeed = _speed; }
		inline float GetCameraTranslationSpeed() const { return m_translationSpeed; }
		inline float GetCameraRotationSpeed() const { return m_rotationSpeed; }

		inline void SetZoomSpeed(float _speed) { m_zoomSpeed = _speed; }
		inline float GetZoomSpeed() const { return m_zoomSpeed; }

		void SetCameraProjection(float _left, float _right, float _bottom, float _top);

	private:
		bool OnMouseScrolled(MouseScrolledEvent& _e);
		bool OnWindowResized(WindowResizeEvent& _e);
	private:
		Ref<OrthographicCamera> m_camera;

		glm::vec3 m_position;
		bool m_canZoom;
		float m_zoomLevel;
		bool m_canRotate;
		float m_rotation;
		float m_translationSpeed;
		float m_rotationSpeed;
		float m_zoomSpeed;
		float m_minZoomLevel;
		float m_maxZoomLevel;
	};
}
#endif