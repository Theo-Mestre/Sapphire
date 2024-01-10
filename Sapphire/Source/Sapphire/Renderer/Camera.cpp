#include "sphpch.h"
#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

#include "Sapphire/Log.h"

namespace sph
{
	OrthographicCamera::OrthographicCamera()
		: m_projectionMatrix(1.0f)
		, m_viewMatrix(1.0f)
		, m_viewProjectionMatrix(1.0f)
	{
		RecalculateViewMatrix();
	}

	OrthographicCamera::OrthographicCamera(float _left = -1.0f, float _right = 1.0f, float _bottom = -1.0f, float _top = -1.0f)
		: m_projectionMatrix(glm::ortho(_left, _right, _bottom, _top, -1.0f, 1.0f))
		, m_viewMatrix()
		, m_viewProjectionMatrix(m_projectionMatrix* m_viewMatrix)
		, m_position(0.0f)
		, m_rotation(0.0f)
	{
		RecalculateViewMatrix();
	}

	void OrthographicCamera::RecalculateViewMatrix()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

		m_viewMatrix = glm::inverse(transform);
		m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
	}
}