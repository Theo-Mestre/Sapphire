#include "sphpch.h"
#include "Material.h"
#include "Texture.h"
#include "Shader.h"

namespace sph
{
	Material::Material()
		: m_material(nullptr)
		, m_texture(nullptr)
	{
		SPH_PROFILE_FUNCTION();

	}

	Material::Material(const std::shared_ptr<Shader>& _shader)
		: m_material(_shader)
		, m_texture(nullptr)
	{
		SPH_PROFILE_FUNCTION();

	}

	Material::~Material()
	{
		SPH_PROFILE_FUNCTION();

	}

	void Material::Bind() const
	{
		SPH_PROFILE_FUNCTION();

		m_material->Bind();
		if (m_texture.get())
			m_texture->Bind();
	}

	void Material::Unbind() const
	{
		SPH_PROFILE_FUNCTION();

		m_material->Unbind();
		if (m_texture)
			m_texture->Unbind();
	}

	Ref<Material> Material::Create(const Ref<Shader>& _shader)
	{
		SPH_PROFILE_FUNCTION();

		return std::make_shared<Material>(_shader);
	}
}