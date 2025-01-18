#include "sphpch.h"
#include "TilemapRenderer.h"

#include "Sapphire/Core/Log.h"
#include "Sapphire/Renderer/Camera.h"
#include "Sapphire/Renderer/Shader.h"
#include "Sapphire/Renderer/UniformBuffer.h"
#include "Sapphire/Renderer/TileMap.h"
#include "Sapphire/Renderer/Texture.h"

namespace sph
{
	void TilemapRenderer::DrawTileMap(const Ref<TileMap>& _tilemap)
	{
		SPH_PROFILE_FUNCTION();

		const glm::vec2 mapSize = _tilemap->GetCellCount();
		const glm::vec2 tileSize = _tilemap->GetCellSize();
		const glm::vec2 mapOffest = tileSize * (mapSize / 2.0f);

		uint32_t i = 0;
		for (auto& layer : _tilemap->GetTileMap())
		{
			float textureIndex = SubmitTexture(_tilemap->GetTextureAtlases()[i]);
			glm::vec2 textureSize = _tilemap->GetTextureAtlases()[i]->GetSize();

			glm::vec2 size = tileSize / textureSize;
			glm::vec2 texCoords[4];

			for (int32_t y = 0; y < mapSize.y; y++)
			{
				for (int32_t x = 0; x < mapSize.x; x++)
				{
					int32_t tileIndex = layer[y * mapSize.x + x];
					if (tileIndex < 0) continue;

					glm::vec3 position(x * tileSize.x - mapOffest.x, 1 - y * tileSize.y + mapOffest.y, 0.0f);

					glm::vec2 coords = _tilemap->GetTilesTextures().at(tileIndex);

					texCoords[0] = { coords.x, coords.y };
					texCoords[1] = { coords.x + size.x, coords.y };
					texCoords[2] = { coords.x + size.x, coords.y + size.y };
					texCoords[3] = { coords.x, coords.y + size.y };

					// Rendering
					CheckBatchState();
					UpdateCurrentQuadVertex(position, tileSize, 0.0f, { 1.0f, 1.0f, 1.0f, 1.0f }, textureIndex, 1.0f, texCoords);
					Renderer::Stats::QuadCount++;
				}
			}
			i++;
		}
	}
}