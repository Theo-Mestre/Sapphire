#ifndef SPH_TEXTURE_H
#define SPH_TEXTURE_H

#include "Sapphire/Core.h"	

namespace sph
{
	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual void Bind(uint32_t _slot = 0) const = 0;
		virtual void Unbind() const = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual uint32_t GetRendererID() const = 0;

		virtual bool operator==(const Texture& _other) const = 0;
	};

	class Texture2D 
		: public Texture
	{
	public:
		static Ref<Texture2D> Create(const std::string& _path);
		static Ref<Texture2D> Create(uint32_t _width, uint32_t _height);
	};
}
#endif