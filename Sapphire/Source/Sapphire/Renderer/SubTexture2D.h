#ifndef SPH_SUBTEXTURE2D_H
#define SPH_SUBTEXTURE2D_H

#include "Texture.h"

namespace sph
{
	class SubTexture2D
	{
	public:

		SubTexture2D(const Ref<Texture2D>& _texture, const glm::vec2& _position, const glm::vec2& _size);

		const Ref<Texture2D> GetTexture() const { return m_texture; }
		const glm::vec2* GetTexCoords() { return m_texCoords; }

		static Ref<SubTexture2D> Create(const Ref<Texture2D>& _texture, const glm::vec2& _index, const glm::vec2& _size);

	private:
		Ref<Texture2D> m_texture;
		glm::vec2 m_texCoords[4];
		glm::vec2 m_size;

	};
}
#endif