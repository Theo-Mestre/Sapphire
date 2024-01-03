#include "sphpch.h"
#include <glad/glad.h>

#include "Shader.h"
#include "Sapphire/Core.h"
#include "Sapphire/Log.h"

namespace sph
{
	Shader::Shader(const std::string& _vertexSource, const std::string& _fragmentSource)
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
			ASSERT(false, "Shader link failure!");

			return;
		}

		// Detach shaders after a successful link.
		glDetachShader(m_rendererID, vertexShader);
		glDetachShader(m_rendererID, fragmentShader);
	}

	Shader::~Shader()
	{
		glDeleteProgram(m_rendererID);
	}

	void Shader::Bind() const
	{
		glUseProgram(m_rendererID);
	}

	void Shader::Unbind() const
	{
		glUseProgram(0);
	}
}