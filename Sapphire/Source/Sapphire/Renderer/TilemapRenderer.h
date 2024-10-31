#ifndef SPH_TILEMAP_RENDERER_H
#define SPH_TILEMAP_RENDERER_H

#include "Sapphire/Renderer/BatchRenderer2D.h"

namespace sph
{
	class TileMap;
	class Shader;

	class TilemapRenderer
		: public BatchRenderer2D
	{
	public:
		void DrawTileMap(const Ref<TileMap>& _tilemap);
	};
}
#endif