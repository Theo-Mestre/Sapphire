#ifndef SPH_SHADER_H
#define SPH_SHADER_H

#include <string>

namespace sph
{
	class Shader
	{
	public:
		Shader() = default;
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		static Shader* Create(const std::string& _vertexSource, const std::string& _fragmentSource);
	};
}
#endif