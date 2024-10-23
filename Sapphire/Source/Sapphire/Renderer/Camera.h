#ifndef SPH_CAMERA_H
#define SPH_CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace sph
{
	class OrthographicCamera
	{
	public:
		OrthographicCamera();
		OrthographicCamera(float _left, float _right, float _bottom, float _top);
		~OrthographicCamera() = default;


		inline const glm::mat4& GetViewMatrix() const { return m_projectionMatrix; }
		inline const glm::mat4& GetProjectionMatrix() const { return m_viewMatrix; }
		inline const glm::mat4& GetViewProjectionMatrix() const { return m_viewProjectionMatrix; }
		inline const glm::vec3& GetPosition() const { return m_position; }
		inline const glm::vec3& GetRotation() const { return m_rotation; }

		inline void SetPosition(float x, float y, float z) { m_position = glm::vec3(x, y, z); RecalculateViewMatrix(); }
		inline void SetPosition(const glm::vec3& position) { m_position = position; RecalculateViewMatrix(); }
		
		inline void SetRotation(float x, float y, float z) { m_rotation = glm::vec3(x, y, z); RecalculateViewMatrix(); }
		inline void SetRotation(const glm::vec3& rotation) { m_rotation = rotation; RecalculateViewMatrix(); }
		
		inline void Rotate(float x, float y, float z) { m_rotation += glm::vec3(x, y, z); RecalculateViewMatrix(); }
		inline void Translate(float x, float y, float z) { m_position += glm::vec3(x, y, z); RecalculateViewMatrix(); }

		inline void SetProjectionMatrix(float left, float right, float bottom, float top) { m_projectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f); RecalculateViewMatrix(); }
		inline void SetViewMatrix(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up) { m_viewMatrix = glm::lookAt(position, target, up); RecalculateViewMatrix(); }

		void RecalculateViewMatrix();
	private:
		glm::mat4 m_projectionMatrix;
		glm::mat4 m_viewMatrix;
		glm::mat4 m_viewProjectionMatrix;

		glm::vec3 m_position;
		glm::vec3 m_rotation;
	};
}
#endif