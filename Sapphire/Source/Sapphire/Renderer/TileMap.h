#pragma once
#include "Sapphire/Core/Core.h"

namespace sph
{
	class Texture2D;
	class SubTexture2D;

	class TileMap
	{
	public:
		TileMap();
		~TileMap();

		void LoadTileMap(uint32_t _textureIndex, const std::string& _path);

		inline const glm::ivec2& GetCellSize() const { return m_cellSize; }
		inline void SetCellSize(const glm::ivec2& cellSize) { m_cellSize = cellSize; }
		inline void SetCellSize(float cellWidth, float cellHeight) { m_cellSize = { cellWidth, cellHeight }; }

		inline const glm::ivec2& GetCellCount() const { return m_cellCount; }
		inline void SetCellCount(const glm::ivec2& cellCount) { m_cellCount = cellCount; }
		inline void SetCellCount(float cellCountX, float cellCountY) { m_cellCount = { cellCountX, cellCountY }; }

		void AddTextureAtlas(uint32_t _textureIndex, const sph::Ref<sph::Texture2D>& _textureAtlas);

		static sph::Ref<TileMap> Create(const std::string& _configPath);

		const auto& GetTilesTextures() const { return m_tileTextures; }
		const auto& GetTextureAtlases() const { return m_textureAtlases; }
		const auto& GetTileMap() const { return m_tileMap; }

	private:
		std::vector<std::string> LoadConfig(const std::string& _configPath);
	private:
		std::unordered_map<uint32_t, sph::Ref<sph::SubTexture2D>> m_tileTextures;
		std::vector<sph::Ref<sph::Texture2D>> m_textureAtlases;
		std::vector<uint32_t> m_tileMap;

		glm::ivec2 m_cellSize;
		glm::ivec2 m_cellCount;
	};
}