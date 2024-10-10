#include "sphpch.h"
#include <glad/glad.h>

#include "Shader.h"
#include "Sapphire/Core/Log.h"
#include "Sapphire/Renderer/RendererAPI.h"

#include "Sapphire/Platform/OpenGL/OpenGLShader.h"

namespace sph
{
	Ref<Shader> Shader::Create(const std::string& _filePath)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:
			ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLShader>(_filePath);
		}

		ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
	Ref<Shader> Shader::Create(const std::string&  _name, const std::string& _vertexSource, const std::string& _fragmentSource)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:
			ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLShader>(_name, _vertexSource, _fragmentSource);
		}

		ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}


	void ShaderLibrary::AddShader(const sph::Ref<Shader>& _shader)
	{
		ASSERT(_shader->GetName() != "", "Shader have no name!")

		m_shaders[_shader->GetName()] = _shader;
	}
	void ShaderLibrary::LoadShader(const std::string& _filePath)
	{
		sph::Ref<Shader> colorShader(Shader::Create(_filePath));
		AddShader(colorShader);
	}
	sph::Ref<Shader> ShaderLibrary::GetShader(const std::string& _name)
	{
		if (Exists(_name))
		{
			return m_shaders[_name];
		}

		ASSERT(false, "Shader " + _name + " not found!");
		return nullptr;
	}
	void ShaderLibrary::RemoveShader(const std::string& _name)
	{
		if (Exists(_name))
		{
			m_shaders.erase(_name);
		}
	}

	void ShaderLibrary::RemoveShader(const sph::Ref<Shader>& _shader)
	{
		RemoveShader(_shader->GetName());
	}
	bool ShaderLibrary::Exists(const std::string& _name) const
	{
		return m_shaders.find(_name) != m_shaders.end();
	}
}