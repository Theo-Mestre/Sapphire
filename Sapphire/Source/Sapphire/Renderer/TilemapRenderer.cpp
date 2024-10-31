#include "sphpch.h"
#include "TilemapRenderer.h"

#include "Sapphire/Core/Log.h"
#include "Sapphire/Renderer/Camera.h"
#include "Sapphire/Renderer/Shader.h"
#include "Sapphire/Renderer/UniformBuffer.h"
#include "Sapphire/Renderer/TileMap.h"

namespace sph
{
	void TilemapRenderer::DrawTileMap(const Ref<TileMap>& _tilemap)
	{
		const glm::vec2 mapSize = _tilemap->GetCellCount();
		const glm::vec2 tileSize = _tilemap->GetCellSize();

		for (auto& layer : _tilemap->GetTileMap())
		{
			const glm::vec2 mapOffest = tileSize * (mapSize / 2.0f);

			for (int32_t y = 0; y < mapSize.y; y++)
			{
				for (int32_t x = 0; x < mapSize.x; x++)
				{
					int32_t tileIndex = layer[y * mapSize.x + x];
					if (tileIndex < 0) continue;

					glm::vec3 position(x * tileSize.x - mapOffest.x, 1 - y * tileSize.y + mapOffest.y, 0.0f);
					DrawQuad(position, tileSize, _tilemap->GetTilesTextures().at(tileIndex));
				}
			}
		}

		//const glm::vec2 mapOffest = { tileSize * (MAP_SIZE_X / 2.0f), tileSize * (MAP_SIZE_Y / 2.0f) };
		//
		//for (uint32_t y = 0; y < MAP_SIZE_Y; y++)
		//{
		//	for (uint32_t x = 0; x < MAP_SIZE_X; x++)
		//	{
		//		int32_t tileIndex = layer.tiles[y * MAP_SIZE_X + x];
		//		if (tileIndex == -1) continue;
		//
		//		glm::vec3 position = { x * tileSize - mapOffest.x ,1 - y * tileSize + mapOffest.y, 0.0f };
		//		_renderer->DrawQuad(position, glm::vec2{ tileSize, tileSize }, layer.subTextures[tileIndex]);
		//	}
		//}
	}
}