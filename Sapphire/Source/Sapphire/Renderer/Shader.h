#ifndef SPH_SHADER_H
#define SPH_SHADER_H

#include <string>

namespace sph
{
	class Shader
	{
	public:
		Shader(const std::string& _vertexSource, const std::string& _fragmentSource);
		~Shader();

		void Bind() const;
		void Unbind() const;
	private:
		uint32_t m_rendererID;
	};
}
#endif