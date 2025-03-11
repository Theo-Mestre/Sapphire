#ifndef SPH_EDITOR_CAMERA_H
#define SPH_EDITOR_CAMERA_H

#include "Sapphire/Renderer/Camera.h"
#include "Sapphire/Events/MouseEvent.h"
#include "Sapphire/Core/Time.h"

namespace sph
{
	class EditorCamera : public Camera
	{
	public:
		EditorCamera() = default;
		EditorCamera(float _fov, float _aspectRatio, float _nearClip, float _farClip);

		void OnUpdate(DeltaTime _dt);
		void OnEvent(Event& _event);

		void ResetTransform();

		inline float GetDistance() const { return m_distance; }
		inline void SetDistance(float _distance) { m_distance = _distance; }

		inline void SetViewportSize(float _width, float _height) { m_viewportWidth = _width; m_viewportHeight = _height; UpdateProjection(); }

		const glm::mat4& GetViewMatrix() const { return m_viewMatrix; }
		glm::mat4 GetViewProjection() const { return m_projection * m_viewMatrix; }

		glm::vec3 GetUpDirection() const;
		glm::vec3 GetRightDirection() const;
		glm::vec3 GetForwardDirection() const;
		const glm::vec3& GetPosition() const { return m_position; }
		glm::quat GetOrientation() const;

		float GetPitch() const { return m_pitch; }
		float GetYaw() const { return m_yaw; }
	private:
		void UpdateProjection();
		void UpdateView();

		bool OnMouseScrolled(MouseScrolledEvent& e);

		void MousePan(const glm::vec2& _delta);
		void MouseRotate(const glm::vec2& _delta);
		void MouseZoom(float _delta);

		glm::vec3 CalculatePosition() const;

		std::pair<float, float> PanSpeed() const;
		float RotationSpeed() const;
		float ZoomSpeed() const;
	private:
		float m_FOV = 45.0f;
		float m_aspectRatio = 1.778f;
		float m_nearClip = 0.1f;
		float m_farClip = 1000.0f;

		glm::mat4 m_viewMatrix;
		glm::vec3 m_position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_focalPoint = { 0.0f, 0.0f, 0.0f };

		glm::vec2 m_initialMousePosition = { 0.0f, 0.0f };

		float m_distance = 10.0f;
		float m_pitch = 0.0f;
		float m_yaw = 0.0f;

		float m_viewportWidth = 1280.0f;
		float m_viewportHeight = 720;
	};
}
#endif