#include "sphpch.h"

#include "Sprite.h"
#include "Sapphire/Renderer/Texture.h"

namespace sph
{
	Sprite::Sprite()
		: Sprite(nullptr, false)
	{
		SPH_PROFILE_FUNCTION();

	}

	Sprite::Sprite(const Ref<Texture2D> _texture, bool _useTextureSize)
		: m_position(0.0f)
		, m_size(1.0f)
		, m_offset(0.5f)
		, m_color(1.0f)
		, m_rotation(0.0f)
		, m_texture(_texture)
	{
		SPH_PROFILE_FUNCTION();

		if (_useTextureSize && _texture != nullptr)
		{
			m_size = glm::vec2(m_texture->GetWidth(), m_texture->GetHeight());
		}
	}

	void Sprite::SetOffset(const glm::vec2& _offset)
	{
		SPH_PROFILE_FUNCTION();

		m_offset.x = std::clamp(_offset.x, 0.0f, 1.0f);
		m_offset.y = std::clamp(_offset.y, 0.0f, 1.0f);
	}

	void Sprite::SetTexture(const Ref<Texture2D> _texture, bool _useTextureSize)
	{
		SPH_PROFILE_FUNCTION();

		m_texture = _texture;

		if (_useTextureSize && _texture != nullptr)
		{
			m_size = m_texture->GetSize();
		}
	}

	const glm::mat4& Sprite::GetTransform() const
	{
		SPH_PROFILE_FUNCTION();

		glm::vec2 offsetPixel = (m_offset - 0.5f) * m_size;

		return glm::translate(glm::mat4(1.0f), m_position - glm::vec3(offsetPixel, 0.0f))
			* glm::rotate(glm::mat4(1.0f), m_rotation, glm::vec3(0.0f, 0.0f, 1.0f))
			* glm::scale(glm::mat4(1.0f), glm::vec3(m_size, 1.0f));
	}

	Ref<Sprite> Sprite::Create(const Ref<Texture2D> _texture, bool _useTextureSize)
	{
		SPH_PROFILE_FUNCTION();

		return CreateRef<Sprite>(_texture, _useTextureSize);
	}
}