#pragma once
#include "Tile.h"
#include <string>
#include <unordered_map>

class Background
{
private:
	GLuint m_TexName;
	unsigned int m_MapWidth;
	unsigned int m_MapHeight;
	std::unordered_map<char, Tile> m_TileLookup;
public:
	Background();
	Tile tiles[900];
	void loadBackground(std::string file, float tileSizeX, float tileSizeY);
	unsigned int GetMapWidth();
	unsigned int GetMapHeight();

};

