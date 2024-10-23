#ifndef SPH_TEXTURE_H
#define SPH_TEXTURE_H

#include "Sapphire/Core/Core.h"	

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

		enum class WrapMode
		{
			None = 0,
			Repeat = 0x2901,
			ClampToEdge = 0x812F,
			ClampToBorder = 0x812D,
			MirroredRepeat = 0x8370,
			MirroredClampToEdge = 0x8742,
			MirroredClampToBorder = 0x8912
		};

		enum class FilterMode
		{
			None = 0,
			Nearest = 0x2600,
			Linear = 0x2601
		};

		struct Properties
		{
			FilterMode MinificationFilter = FilterMode::Nearest;
			FilterMode MagnificationFilter = FilterMode::Nearest;
			WrapMode WrapModeS = WrapMode::ClampToEdge;
			WrapMode WrapModeT = WrapMode::ClampToEdge;
		};
	};

	class Texture2D 
		: public Texture
	{
	public:
		static Ref<Texture2D> Create(const std::string& _path, const Properties& _properties = Properties());
		static Ref<Texture2D> Create(uint32_t _width, uint32_t _height, const Properties& _properties = Properties());

		virtual void SetData(void* _data, uint32_t _size) = 0;
	};
}
#endif