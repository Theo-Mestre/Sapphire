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
		OpenGLShader(const std::string& _filePath);
		OpenGLShader(const std::string& _name, const std::string& _vertexSource, const std::string& _fragmentSource);
		virtual ~OpenGLShader() override;

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void UploadUniformInt(const std::string& _name, int _value);
		virtual void UploadUniformIntArray(const std::string& _name, int* _values, uint32_t _count);
		virtual void UploadUniformFloat(const std::string& _name, float _value);
		virtual void UploadUniformFloat2(const std::string& _name, const glm::vec2& _value);
		virtual void UploadUniformFloat3(const std::string& _name, const glm::vec3& _value);
		virtual void UploadUniformFloat4(const std::string& _name, const glm::vec4& _value);
		virtual void UploadUniformMat3f(const std::string& _name, const glm::mat3& _matrix);
		virtual void UploadUniformMat4f(const std::string& _name, const glm::mat4& _matrix);

		virtual void SetInt(const std::string& _name, int _value) override;
		virtual void SetIntArray(const std::string& _name, int* _values, uint32_t _count) override;
		virtual void SetFloat(const std::string& _name, float _value) override;
		virtual void SetFloat2(const std::string& _name, const glm::vec2& _value) override;
		virtual void SetFloat3(const std::string& _name, const glm::vec3& _value) override;
		virtual void SetFloat4(const std::string& _name, const glm::vec4& _value) override;
		virtual void SetMat3(const std::string& _name, const glm::mat3& _matrix) override;
		virtual void SetMat4(const std::string& _name, const glm::mat4& _matrix) override;
	private:
		int32_t GetUniformLocation(const std::string& _name) const;

		// Shader file parsing
		std::string ParseShaderFile(const std::string& _filePath);
		std::string ComputeShaderName(const std::string& _filePath);
		// Shader compilation and linking
		std::pair<std::string, std::string> PreProcessShaderSource(const std::string& _shaderSource);
		uint32_t CompileShader(uint32_t _type, const std::string& _source);
		uint32_t LinkProgram(uint32_t _vertexShader, uint32_t _fragmentShader);
	private:
		mutable std::unordered_map<std::string, int32_t> m_uniformLocationCache;
	};
}

#endif