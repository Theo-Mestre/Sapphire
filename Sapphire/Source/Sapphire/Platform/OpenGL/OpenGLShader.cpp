#include "sphpch.h"
#include <glad/glad.h>

#include "OpenGLShader.h"
#include "Sapphire/Log.h"

namespace sph
{
	OpenGLShader::OpenGLShader(const std::string& _vertexSource, const std::string& _fragmentSource)
		: m_rendererID(0)
	{
		// Create an empty vertex shader handle
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		const GLchar* source = _vertexSource.c_str();
		glShaderSource(vertexShader, 1, &source, 0);

		// Compile the vertex shader
		glCompileShader(vertexShader);

		GLint isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			// Fetch the info log
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

			// Delete the shader
			glDeleteShader(vertexShader);

			LogError("{0}", infoLog.data());
			ASSERT(false, "Vertex shader compilation failure!");
			return;
		}

		// Create an empty fragment shader handle
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		source = _fragmentSource.c_str();
		glShaderSource(fragmentShader, 1, &source, 0);

		// Compile the fragment shader
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			// Fetch the info log
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

			// Delete the shaders.
			glDeleteShader(fragmentShader);
			glDeleteShader(vertexShader);

			LogError("{0}", infoLog.data());
			ASSERT(false, "Fragment shader compilation failure!");
			return;
		}

		// Create the program handle
		m_rendererID = glCreateProgram();

		// Attach the shaders to the program
		glAttachShader(m_rendererID, vertexShader);
		glAttachShader(m_rendererID, fragmentShader);

		// Link the program
		glLinkProgram(m_rendererID);

		GLint isLinked = 0;
		glGetProgramiv(m_rendererID, GL_LINK_STATUS, (int*)&isLinked);

		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(m_rendererID, GL_INFO_LOG_LENGTH, &maxLength);

			// Fetch the info log
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(m_rendererID, maxLength, &maxLength, &infoLog[0]);

			// Delete the shaders and program
			glDeleteProgram(m_rendererID);
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			LogError("{0}", infoLog.data());
			ASSERT(false, "OpenGLShader link failure!");

			return;
		}

		// Read the uniform names and cache them
		GLint numUniforms = 0;
		GLint maxNameLength = 0;

		glGetProgramiv(m_rendererID, GL_ACTIVE_UNIFORMS, &numUniforms);
		glGetProgramiv(m_rendererID, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxNameLength);

		std::vector<GLchar> nameData(maxNameLength);

		for (int32_t uniformIndex = 0; uniformIndex < numUniforms; uniformIndex++)
		{
			GLint arraySize = 0;
			GLenum type = 0;
			GLsizei actualLength = 0;
			glGetActiveUniform(m_rendererID, uniformIndex, static_cast<GLsizei>(nameData.size()), &actualLength, &arraySize, &type, &nameData[0]);

			std::string name((char*)&nameData[0], actualLength);
			GLuint location = glGetUniformLocation(m_rendererID, name.c_str());
			m_uniformLocationCache[name] = location;
		}

		// Detach shaders after a successful link.
		glDetachShader(m_rendererID, vertexShader);
		glDetachShader(m_rendererID, fragmentShader);
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

	void OpenGLShader::SetUniformInt(const std::string& _name, int _value)
	{
		glUniform1i(GetUniformLocation(_name), _value);
	}

	void OpenGLShader::SetUniformFloat(const std::string& _name, float _value)
	{
		glUniform1f(GetUniformLocation(_name), _value);
	}

	void OpenGLShader::SetUniformFloat2(const std::string& _name, const glm::vec2& _value)
	{
		glUniform2f(GetUniformLocation(_name), _value.x, _value.y);
	}

	void OpenGLShader::SetUniformFloat3(const std::string& _name, const glm::vec3& _value)
	{
		glUniform3f(GetUniformLocation(_name), _value.x, _value.y, _value.z);
	}

	void OpenGLShader::SetUniformFloat4(const std::string& _name, const glm::vec4& _value)
	{
		glUniform4f(GetUniformLocation(_name), _value.x, _value.y, _value.z, _value.w);
	}

	void OpenGLShader::SetUniformMat3f(const std::string& _name, const glm::mat3& _matrix)
	{
		glUniformMatrix3fv(GetUniformLocation(_name), 1, GL_FALSE, glm::value_ptr(_matrix));
	}

	void OpenGLShader::SetUniformMat4f(const std::string& _name, const glm::mat4& _matrix)
	{
		glUniformMatrix4fv(GetUniformLocation(_name), 1, GL_FALSE, glm::value_ptr(_matrix));
	}
	int32_t OpenGLShader::GetUniformLocation(const std::string& _name) const
	{
		if (m_uniformLocationCache.find(_name) != m_uniformLocationCache.end())
			return m_uniformLocationCache[_name];

		ASSERT(false, "Uniform {0} doesn't exist!", _name);
		return -1;
	}
}