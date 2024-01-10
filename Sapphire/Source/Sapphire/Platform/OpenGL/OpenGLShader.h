#ifndef SPH_OPENGL_SHADER_H
#define SPH_OPENGL_SHADER_H

#include <glm/glm.hpp>

#include "Sapphire/Renderer/Shader.h"

namespace sph
{
	class OpenGLShader
		: public Shader
	{
	public:
		OpenGLShader(const std::string& _vertexSource, const std::string& _fragmentSource);
		virtual ~OpenGLShader() override;

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetUniformInt(const std::string& _name, int _value);
		virtual void SetUniformFloat(const std::string& _name, float _value);
		virtual void SetUniformFloat2(const std::string& _name, const glm::vec2& _value);
		virtual void SetUniformFloat3(const std::string& _name, const glm::vec3& _value);
		virtual void SetUniformFloat4(const std::string& _name, const glm::vec4& _value);
		virtual void SetUniformMat3f(const std::string& _name, const glm::mat3& _matrix);
		virtual void SetUniformMat4f(const std::string& _name, const glm::mat4& _matrix);
	private:
		int32_t GetUniformLocation(const std::string& _name) const;
	private:
		mutable std::unordered_map<std::string, int32_t> m_uniformLocationCache;
		uint32_t m_rendererID;
	};
}

#endif