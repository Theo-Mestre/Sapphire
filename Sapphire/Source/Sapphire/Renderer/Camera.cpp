#include "sphpch.h"
#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

#include "Sapphire/Core/Log.h"

namespace sph
{
	OrthographicCamera::OrthographicCamera()
		: m_projectionMatrix(1.0f)
		, m_viewMatrix(1.0f)
		, m_viewProjectionMatrix(1.0f)
	{
		SPH_PROFILE_FUNCTION();

		RecalculateViewMatrix();
	}

	OrthographicCamera::OrthographicCamera(float _left = -1.0f, float _right = 1.0f, float _bottom = -1.0f, float _top = -1.0f)
		: m_projectionMatrix(glm::ortho(_left, _right, _bottom, _top, -1.0f, 1.0f))
		, m_viewMatrix()
		, m_viewProjectionMatrix(m_projectionMatrix* m_viewMatrix)
		, m_viewportSize(_right - _left, _top - _bottom)
		, m_origin((_right + _left) / 2.0f, (_top + _bottom) / 2.0f)
		, m_position(0.0f)
		, m_rotation(0.0f)
		, m_scale(1.0f)
	{
		SPH_PROFILE_FUNCTION();

		RecalculateViewMatrix();
	}

	void OrthographicCamera::RecalculateViewMatrix()
	{
		SPH_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)) *
			glm::scale(glm::mat4(1.0f), glm::vec3(m_scale));

		m_viewMatrix = glm::inverse(transform);
		m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
	}

	glm::vec2 OrthographicCamera::WorldToScreen(const glm::vec3& _point) const
	{
		SPH_PROFILE_FUNCTION();

		glm::vec4 viewPoint = m_viewMatrix * glm::vec4(_point, 1.0f);

		glm::vec4 projectedPoint = m_projectionMatrix * viewPoint;
		glm::vec2 ndc = glm::vec2(projectedPoint) / projectedPoint.w;

		glm::vec2 point = ndc * m_viewportSize + m_origin;
		//point.y = m_viewportSize.y - point.y;

		return glm::vec3(point.x, point.y, _point.z);
	}

	glm::vec3 OrthographicCamera::ScreenToWorld(const glm::vec2& _point) const
	{
		SPH_PROFILE_FUNCTION();

		glm::vec4 ndcPoint =
		{
			(2.0f * _point.x) / m_viewportSize.x - 1.0f,
			1.0f - (2.0f * _point.y) / m_viewportSize.y,
			0.0f,
			1.0f
		};

		glm::vec4 viewSpacePoint = glm::inverse(m_projectionMatrix) * ndcPoint;
		if (viewSpacePoint.w != 0.0f) viewSpacePoint /= viewSpacePoint.w;

		return glm::vec3(glm::inverse(m_viewMatrix) * viewSpacePoint);
	}

	Ref<OrthographicCamera> OrthographicCamera::Create(float _left, float _right, float _bottom, float _top)
	{
		SPH_PROFILE_FUNCTION();

		return CreateRef<OrthographicCamera>(_left, _right, _bottom, _top);
	}
}