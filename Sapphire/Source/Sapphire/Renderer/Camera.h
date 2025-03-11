#ifndef SPH_CAMERA_H
#define SPH_CAMERA_H

#include <glm/glm.hpp>

#include "Sapphire/Core/Core.h"

namespace sph
{
	class Camera
	{
	public:
		Camera() = default;
		Camera(const glm::mat4& _projection)
			: m_projection(_projection) {
		}
		virtual ~Camera() = default;

		inline const glm::mat4& GetProjection() const { return m_projection; }
		inline void SetProjection(const glm::mat4& _projection) { m_projection = _projection; };

	protected:
		glm::mat4 m_projection = glm::mat4(1.0f);
	};
}
#endif