#ifndef SPH_SPRITE_H
#define SPH_SPRITE_H

#include "Sapphire/Core/Core.h"

namespace sph
{
	class Texture2D;

	class Sprite
	{
	public:
		Sprite();
		Sprite(const Ref<Texture2D> _texture, bool _useTextureSize = true);

		// Getters and Setters
		inline void SetPosition(const glm::vec3& _position) { m_position = _position; }
		inline void SetPosition(const glm::vec2& _position) { m_position = { _position,m_position.z }; }
		inline const glm::vec3& GetPosition() const { return m_position; }
		inline const glm::vec2& GetPosition2D() const { return m_position; }

		inline void SetSize(const glm::vec2& _size) { m_size = _size; }
		inline const glm::vec2& GetSize() const { return m_size; }

		void SetOffset(const glm::vec2& _offset);
		inline const glm::vec2& GetOffset() const { return m_offset; }

		inline void SetColor(const glm::vec4& _color) { m_color = _color; }
		inline const glm::vec4& GetColor() const { return m_color; }

		inline void SetRotation(float _rotation) { m_rotation = _rotation; }
		inline float GetRotation() const { return m_rotation; }

		void SetTexture(const Ref<Texture2D> _texture, bool _useTextureSize = true);
		inline const Ref<Texture2D>& GetTexture() const { return m_texture; }

		const glm::mat4& GetTransform() const;

		static Ref<Sprite> Create(const Ref<Texture2D> _texture, bool _useTextureSize = true);
	private:
		glm::vec3 m_position;
		glm::vec2 m_size;
		glm::vec2 m_offset;
		glm::vec4 m_color;
		float m_rotation;
		Ref<Texture2D> m_texture;
	};
}

#endif