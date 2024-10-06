#include "sphpch.h"
#include "SubTexture2D.h"
#include "Sapphire/Core/Log.h"

namespace sph
{
	SubTexture2D::SubTexture2D(const Ref<Texture2D>& _texture, const glm::vec2& _position, const glm::vec2& _size)
		: m_texture(_texture)
		, m_size(_size)
	{
		m_texCoords[0] = { _position.x, _position.y };
		m_texCoords[1] = { _position.x + _size.x, _position.y };
		m_texCoords[2] = { _position.x + _size.x, _position.y + _size.y };
		m_texCoords[3] = { _position.x, _position.y + _size.y };
	}

	Ref<SubTexture2D> SubTexture2D::Create(const Ref<Texture2D>& _texture, const glm::vec2& _index, const glm::vec2& _size)
	{
#		ifdef DEBUG
		if (_index.x < 0 || _index.y < 0 || _size.x <= 0 || _size.y <= 0
			|| _index.x * _size.x > _texture->GetWidth() || _index.y * _size.y > _texture->GetHeight())
		{
			ASSERT(false, "Invalid index or size");
		}
#		endif

		glm::vec2 texSize = { _texture->GetWidth(), _texture->GetHeight() };

		return CreateRef<SubTexture2D>(_texture, glm::vec2{ _index.x * _size.x, _index.y * _size.y } / texSize, _size / texSize);
	}
}