#include "sphpch.h"
#include <glad/glad.h>

#include "OpenGLShader.h"
#include "Sapphire/Core/Log.h"

namespace sph
{
	OpenGLShader::OpenGLShader(const std::string& _filePath)
	{
		std::string source = ParseShaderFile(_filePath);
		auto [vertexSource, fragmentSource] = PreProcessShaderSource(source);

		GLuint vertexShader = CompileShader(GL_VERTEX_SHADER, vertexSource);
		GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);

		m_rendererID = LinkProgram(vertexShader, fragmentShader);

		m_name = ComputeShaderName(_filePath);
	}

	OpenGLShader::OpenGLShader(const std::string& _name, const std::string& _vertexSource, const std::string& _fragmentSource)
	{
		GLuint vertexShader = CompileShader(GL_VERTEX_SHADER, _vertexSource);
		GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, _fragmentSource);

		m_rendererID = LinkProgram(vertexShader, fragmentShader);

		m_name = _name;
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_rendererID);
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_rendererID);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	int32_t OpenGLShader::GetUniformLocation(const std::string& _name) const
	{
		// Check if the uniform location is already cached
		if (m_uniformLocationCache.find(_name) != m_uniformLocationCache.end())
		{
			return m_uniformLocationCache[_name];
		}

		// If not, get the uniform location and cache it
		if (glGetUniformLocation(m_rendererID, _name.c_str()) != -1)
		{
			m_uniformLocationCache[_name] = glGetUniformLocation(m_rendererID, _name.c_str());
			return m_uniformLocationCache[_name];
		}

		ASSERT(false, "Uniform " + _name + " doesn't exist!");
		return -1;
	}

	std::string OpenGLShader::ParseShaderFile(const std::string& _filePath)
	{
		std::ifstream file(_filePath, std::ios::binary);
		if (!file.is_open())
		{
			LogError("Failed to open shader file: {0}", _filePath);
			ASSERT(false, "Failed to open shader file!");
			return std::string();
		}

		file.seekg(0, std::ios::end);
		size_t size = file.tellg();

		std::string source(size, ' ');

		file.seekg(0);
		file.read(&source[0], size);

		file.close();

		return source;
	}

	std::string OpenGLShader::ComputeShaderName(const std::string& _filePath)
	{
		size_t lastSlash = _filePath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;

		size_t lastDot = _filePath.rfind('.');
		size_t count = lastDot == std::string::npos ? _filePath.size() - lastSlash : lastDot - lastSlash;

		return _filePath.substr(lastSlash, count);
	}

	std::pair<std::string, std::string> OpenGLShader::PreProcessShaderSource(const std::string& _shaderSource)
	{
		static constexpr const char* typeTokenVertex = "#type vertex";
		static constexpr const char* typeTokenFragment = "#type fragment";

		std::pair<std::string, std::string> shaderSources;

		size_t vertexShaderStart = _shaderSource.find("#type vertex");
		size_t fragmentShaderStart = _shaderSource.find("#type fragment");

		vertexShaderStart += strlen(typeTokenVertex);

		shaderSources.first = _shaderSource.substr(vertexShaderStart, fragmentShaderStart - vertexShaderStart);
		shaderSources.second = _shaderSource.substr(fragmentShaderStart + strlen(typeTokenFragment));

		return shaderSources;
	}

	uint32_t OpenGLShader::CompileShader(uint32_t _type, const std::string& _source)
	{
		GLuint colorShader = glCreateShader(_type);

		const GLchar* source = _source.c_str();
		glShaderSource(colorShader, 1, &source, 0);

		// Compile the shader
		glCompileShader(colorShader);

		GLint isCompiled = 0;
		glGetShaderiv(colorShader, GL_COMPILE_STATUS, &isCompiled);

		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(colorShader, GL_INFO_LOG_LENGTH, &maxLength);

			// Fetch the info log
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(colorShader, maxLength, &maxLength, &infoLog[0]);

			// Delete the shader
			glDeleteShader(colorShader);

			LogError("{0}", infoLog.data());
			ASSERT(false, "OpenGLShader compilation failure!");
			return 0;
		}
		return colorShader;
	}

	uint32_t OpenGLShader::LinkProgram(uint32_t _vertexShader, uint32_t _fragmentShader)
	{
		GLuint program = glCreateProgram();

		// Attach the shaders to the program
		glAttachShader(program, _vertexShader);
		glAttachShader(program, _fragmentShader);

		// Link the program
		glLinkProgram(program);

		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);

		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// Fetch the info log
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// Delete the shaders and program
			glDeleteProgram(program);
			glDeleteShader(_vertexShader);
			glDeleteShader(_fragmentShader);

			LogError("{0}", infoLog.data());
			ASSERT(false, "OpenGLShader link failure!");

			return -1;
		}
		return program;
	}

#	pragma region SetUniforms
	void OpenGLShader::UploadUniformInt(const std::string& _name, int _value)
	{
		glUniform1i(GetUniformLocation(_name), _value);
	}

	void OpenGLShader::UploadUniformInt2(const std::string& _name, const glm::ivec2& _value)
	{
		glUniform2i(GetUniformLocation(_name), _value.x, _value.y);
	}

	void OpenGLShader::UploadUniformIntArray(const std::string& _name, int* _values, uint32_t _count)
	{
		glUniform1iv(GetUniformLocation(_name), _count, _values);
	}

	void OpenGLShader::UploadUniformFloat(const std::string& _name, float _value)
	{
		glUniform1f(GetUniformLocation(_name), _value);
	}

	void OpenGLShader::UploadUniformFloat2(const std::string& _name, const glm::vec2& _value)
	{
		glUniform2f(GetUniformLocation(_name), _value.x, _value.y);
	}

	void OpenGLShader::UploadUniformFloat3(const std::string& _name, const glm::vec3& _value)
	{
		glUniform3f(GetUniformLocation(_name), _value.x, _value.y, _value.z);
	}

	void OpenGLShader::UploadUniformFloat4(const std::string& _name, const glm::vec4& _value)
	{
		glUniform4f(GetUniformLocation(_name), _value.x, _value.y, _value.z, _value.w);
	}

	void OpenGLShader::UploadUniformMat3f(const std::string& _name, const glm::mat3& _matrix)
	{
		glUniformMatrix3fv(GetUniformLocation(_name), 1, GL_FALSE, glm::value_ptr(_matrix));
	}

	void OpenGLShader::UploadUniformMat4f(const std::string& _name, const glm::mat4& _matrix)
	{
		glUniformMatrix4fv(GetUniformLocation(_name), 1, GL_FALSE, glm::value_ptr(_matrix));
	}

	void OpenGLShader::SetInt(const std::string& _name, int _value)
	{
		UploadUniformInt(_name, _value);
	}

	void OpenGLShader::SetInt2(const std::string& _name, const glm::ivec2& _value)
	{
		UploadUniformInt2(_name, _value);
	}

	void OpenGLShader::SetIntArray(const std::string& _name, int* _values, uint32_t _count)
	{
		UploadUniformIntArray(_name, _values, _count);
	}

	void OpenGLShader::SetFloat(const std::string& _name, float _value)
	{
		UploadUniformFloat(_name, _value);
	}

	void OpenGLShader::SetFloat2(const std::string& _name, const glm::vec2& _value)
	{
		UploadUniformFloat2(_name, _value);
	}

	void OpenGLShader::SetFloat3(const std::string& _name, const glm::vec3& _value)
	{
		UploadUniformFloat3(_name, _value);
	}

	void OpenGLShader::SetFloat4(const std::string& _name, const glm::vec4& _value)
	{
		UploadUniformFloat4(_name, _value);
	}

	void OpenGLShader::SetMat3(const std::string& _name, const glm::mat3& _matrix)
	{
		UploadUniformMat3f(_name, _matrix);
	}

	void OpenGLShader::SetMat4(const std::string& _name, const glm::mat4& _matrix)
	{
		UploadUniformMat4f(_name, _matrix);
	}
#pragma endregion
}