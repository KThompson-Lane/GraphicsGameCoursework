#pragma once
#include "Tile.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "..\glm\gtx\hash.hpp"
#include <string>
#include <map>
#include <unordered_map>
#include <vector>



class Background
{
private:
	GLuint m_TexName;
	unsigned int m_MapWidth;
	unsigned int m_MapHeight;
	std::unordered_map<char, Tile> m_TileLookup;
public:
	Background();
	std::vector<Tile> dirtTiles;
	std::unordered_map<glm::vec2, Tile> trackTiles;
	void loadBackground(std::string file, float tileSizeX, float tileSizeY);
	unsigned int GetMapWidth();
	unsigned int GetMapHeight();

};
