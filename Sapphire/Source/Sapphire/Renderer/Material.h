#ifndef SPH_MATERIAL_H
#define SPH_MATERIAL_H

#include "Sapphire/Core/Core.h"

namespace sph
{
	class Shader;
	class Texture;

	class Material
	{
	public:
		Material();
		Material(const std::shared_ptr<Shader>& _shader);
		~Material();

		inline void SetShader(const std::shared_ptr<Shader>& _shader) { m_material = _shader; }
		inline std::shared_ptr<Shader> GetShader() const { return m_material; }

		inline void SetTexture(const std::shared_ptr<Texture>& _texture) { m_texture = _texture; }
		inline std::shared_ptr<Texture> GetTexture() const { return m_texture; }

		void Bind() const;
		void Unbind() const;

		static Ref<Material> Create(const Ref<Shader>& _shader);

	private:
		std::shared_ptr<Shader> m_material;
		std::shared_ptr<Texture> m_texture;
	};
}
#endif