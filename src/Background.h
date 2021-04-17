#pragma once
#include "Tile.h"
#include "Checkpoint.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "..\glm\gtx\hash.hpp"
#include <string>
#include <map>
#include <unordered_map>
#include <vector>



class Background
{
private:
	GLuint m_TrackSheetTex;
	GLuint m_CheckpointTex;

	unsigned int m_MapWidth;
	unsigned int m_MapHeight;
	std::unordered_map<char, Tile> m_TileLookup;
	std::unordered_map<char, Checkpoint> m_EntityLookup;
public:
	Background();
	std::vector<Tile> dirtTiles;
	std::unordered_map<glm::vec2, Tile> trackTiles;
	std::unordered_map<glm::vec2, Checkpoint> checkpoints;

	void loadBackground();
	unsigned int GetMapWidth();
	unsigned int GetMapHeight();

};
