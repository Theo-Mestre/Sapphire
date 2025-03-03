#include "sphpch.h"
#include "SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace sph 
{
	SceneCamera::SceneCamera()
	{
		RecalculateProjection();
	}

	void SceneCamera::SetPerspective(float _verticalFOV, float _nearClip, float _farClip)
	{
		m_projectionType = ProjectionType::Perspective;
		m_perspectiveFOV = _verticalFOV;
		m_perspectiveNear = _nearClip;
		m_perspectiveFar = _farClip;
		RecalculateProjection();
	}

	void SceneCamera::SetOrthographic(float _size, float _nearClip, float _farClip)
	{
		m_orthographicSize = _size;
		m_orthographicNear = _nearClip;
		m_orthographicFar = _farClip;
		RecalculateProjection();
	}

	void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		m_aspectRatio = (float)width / (float)height;
		RecalculateProjection();
	}

	void SceneCamera::RecalculateProjection()
	{
		if (m_projectionType == ProjectionType::Perspective)
		{
			m_projection = glm::perspective(m_perspectiveFOV, m_aspectRatio, m_perspectiveNear, m_perspectiveFar);
			return;
		}

		float orthoLeft = -m_orthographicSize * m_aspectRatio * 0.5f;
		float orthoRight = m_orthographicSize * m_aspectRatio * 0.5f;
		float orthoBottom = -m_orthographicSize * 0.5f;
		float orthoTop = m_orthographicSize * 0.5f;

		m_projection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, m_orthographicNear, m_orthographicFar);
	}

}
