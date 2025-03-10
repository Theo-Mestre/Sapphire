#ifndef SPH_MATHS_H
#define SPH_MATHS_H

namespace sph
{
	class Maths
	{
	public:
		static bool DecomposeTransform(const glm::mat4& _transform, glm::vec3& _outTranslation, glm::vec3& _outRotation, glm::vec3& _outScale);
	};
}
#endif