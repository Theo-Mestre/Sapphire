#include "sphpch.h"
#include "Material.h"
#include "Texture.h"
#include "Shader.h"

namespace sph
{
	Material::Material()
		: m_shader(nullptr)
		, m_texture(nullptr)
	{
	}

	Material::Material(const std::shared_ptr<Shader>& _shader)
		: m_shader(_shader)
		, m_texture(nullptr)
	{
	}

	Material::~Material()
	{
	}

	void Material::Bind() const
	{
		m_shader->Bind();
		if (m_texture.get())
			m_texture->Bind();
	}

	void Material::Unbind() const
	{
		m_shader->Unbind();
		if (m_texture)
			m_texture->Unbind();
	}
}