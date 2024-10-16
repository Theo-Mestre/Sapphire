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
		OrthographicCameraController(float _aspectRatio, bool _m_rotate = false);

		void OnUpdate(DeltaTime _dt);
		void OnEvent(Event& _e);

		inline OrthographicCamera& GetCamera() { return m_camera; }
		inline const OrthographicCamera& GetCamera() const { return m_camera; }

		inline float GetZoomLevel() const { return m_zoomLevel; }
		inline void SetZoomLevel(float _level) { m_zoomLevel = _level; }

		inline float GetAspectRatio() const { return m_aspectRatio; }
		inline void SetAspectRatio(float _aspectRatio);

		inline void SetCameraPosition(const glm::vec3& _position) { m_position = _position; m_camera.SetPosition(_position); }
		inline const glm::vec3& GetCameraPosition() const { return m_position; }

		inline void SetCameraRotation(float _rotation) { m_rotation = _rotation; m_camera.SetRotation(0.0f, 0.0f, _rotation); }
		inline float GetCameraRotation() const { return m_rotation; }

		inline void SetCameraTranslationSpeed(float _speed) { m_translationSpeed = _speed; }
		inline void SetCameraRotationSpeed(float _speed) { m_rotationSpeed = _speed; }
		inline float GetCameraTranslationSpeed() const { return m_translationSpeed; }
		inline float GetCameraRotationSpeed() const { return m_rotationSpeed; }

		inline void SetZoomSpeed(float _speed) { m_zoomSpeed = _speed; }
		inline float GetZoomSpeed() const { return m_zoomSpeed; }

		void SetCameraProjection(float _left, float _right, float _bottom, float _top);

		inline void SetScreenSize(glm::vec2 _screenSize) { m_screenSize = _screenSize; }
		inline void SetScreenSize(float _width, float _height) { m_screenSize = { _width, _height }; }
		inline glm::vec2 GetScreenSize() const { return m_screenSize; }
	private:
		bool OnMouseScrolled(MouseScrolledEvent& _e);
		bool OnWindowResized(WindowResizeEvent& _e);
	private:
		float m_aspectRatio;
		float m_zoomLevel;
		OrthographicCamera m_camera;

		glm::vec2 m_screenSize;
		glm::vec3 m_position;
		bool m_rotate;
		float m_rotation;
		float m_translationSpeed;
		float m_rotationSpeed;
		float m_zoomSpeed;
		float m_minZoomLevel;
		float m_maxZoomLevel;
	};
}
#endif