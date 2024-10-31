#include <sphpch.h>

#include "TileMap.h"

#include "Sapphire/Core/Log.h"
#include "Sapphire/Renderer/SubTexture2D.h"
#include "Sapphire/Renderer/Texture.h"

namespace sph
{

	TileMap::TileMap()
		: m_tileTextures()
		, m_textureAtlases()
		, m_tileMap()
		, m_cellSize(0)
		, m_cellCount(0)
	{

	}

	TileMap::~TileMap()
	{

	}

	void TileMap::LoadTileMap(uint32_t _textureIndex, const std::string& _path)
	{
		// Check if cell size and texture are set
		if (m_cellSize.x == 0 || m_cellSize.y == 0)
		{
			ASSERT(false, "Cell size is not set");
			return;
		}

		if (m_textureAtlases.size() <= _textureIndex)
		{
			ASSERT(false, "Texture atlas is not set");
			return;
		}

		// Open file
		std::ifstream file(_path);

		// Exit if the file is not open
		if (!file.is_open())
		{
			ASSERT(false, "Failed to open file");
			return;
		}

		// Read the file
		std::string line;
		uint32_t row = 0;

		m_tileMap.reserve(m_tileMap.size() + 1);
		std::vector<uint32_t> tilemap;
		
		while (std::getline(file, line))
		{
			std::stringstream ss(line);
			std::string cell;
			int32_t col = 0;
			while (std::getline(ss, cell, ','))
			{
				tilemap.push_back(std::stoi(cell));
				col++;
			}
			row++;
			m_cellCount.x = std::max(m_cellCount.x, col);
		}
		m_cellCount.y = row;
		m_tileMap.emplace_back(tilemap);

		file.close();

		glm::ivec2 cellPerSpritesheet = { m_textureAtlases[_textureIndex]->GetWidth() / m_cellSize.x, m_textureAtlases[_textureIndex]->GetHeight() / m_cellSize.y };

		for (auto value : tilemap)
		{
			if (value == -1) continue;

			if (m_tileTextures.find(value) != m_tileTextures.end()) continue;

			glm::vec2 index = { value % cellPerSpritesheet.x, cellPerSpritesheet.y - (value / cellPerSpritesheet.x) - 1 };
			m_tileTextures[value] = sph::SubTexture2D::Create(m_textureAtlases[_textureIndex], index, m_cellSize);
		}
	}

	void TileMap::AddTextureAtlas(uint32_t _textureIndex, const sph::Ref<sph::Texture2D>& _textureAtlas)
	{
		if (m_textureAtlases.size() <= _textureIndex)
		{
			m_textureAtlases.resize(_textureIndex + 1);
		}

		m_textureAtlases[_textureIndex] = _textureAtlas;
	}

	sph::Ref<TileMap> TileMap::Create(const std::string& _configPath)
	{
		sph::Ref<TileMap> tileMap = sph::CreateRef<TileMap>();

		auto mapPaths = tileMap->LoadConfig(_configPath);

		if (mapPaths.empty())
		{
			ASSERT(false, "Failed to load config");
			return nullptr;
		}

		for (uint32_t i = 0; i < mapPaths.size() / 2; i++)
		{
			tileMap->AddTextureAtlas(i, sph::Texture2D::Create(mapPaths[mapPaths.size() / 2 + i]));
			tileMap->LoadTileMap(i, mapPaths[i]);
		}
		return tileMap;
	}

	std::vector<std::string> TileMap::LoadConfig(const std::string& _configPath)
	{
		std::ifstream file(_configPath);

		if (!file.is_open())
		{
			ASSERT(false, "Failed to open file");
			return {};
		}

		std::string line;

		// Read map count
		std::getline(file, line);
		uint32_t mapNumber = std::stoi(line);

		// Read map paths
		std::vector<std::string> mapPaths;
		mapPaths.reserve(mapNumber * 2);
		for (uint32_t i = 0; i < mapNumber * 2; i++)
		{
			std::getline(file, line);
			mapPaths.emplace(mapPaths.begin() + i, line);
		}

		// Read cell size
		std::getline(file, line);

		std::string cellSize;
		std::stringstream ss(line);

		std::getline(ss, cellSize, ',');
		m_cellSize.x = std::stoi(cellSize);
		std::getline(ss, cellSize, ',');
		m_cellSize.y = std::stoi(cellSize);

		file.close();

		return mapPaths;
	}
}