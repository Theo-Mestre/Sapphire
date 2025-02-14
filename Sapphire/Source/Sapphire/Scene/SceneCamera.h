#pragma once

#include "Sapphire/Renderer/Camera.h"

namespace sph
{
	class SceneCamera 
		: public Camera
	{
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		void SetOrthographic(float size, float nearClip, float farClip);

		void SetViewportSize(uint32_t width, uint32_t height);

		float GetOrthographicSize() const { return m_orthographicSize; }
		void SetOrthographicSize(float size) { m_orthographicSize = size; RecalculateProjection(); }

		float GetOrthographicNear() const { return m_orthographicNear; }
		void SetOrthographicNear(float nearClip) { m_orthographicNear = nearClip; RecalculateProjection(); }

		float GetOrthographicFar() const { return m_orthographicFar; }
		void SetOrthographicFar(float farClip) { m_orthographicFar = farClip; RecalculateProjection(); }

		float GetAspectRatio() const { return m_aspectRatio; }
		void SetAspectRatio(float aspectRatio) { m_aspectRatio = aspectRatio; RecalculateProjection(); }
	private:
		void RecalculateProjection();
	private:
		float m_orthographicSize = 10.0f;
		float m_orthographicNear = -1.0f;
		float m_orthographicFar = 1.0f;

		float m_aspectRatio = 0.0f;
	};
}