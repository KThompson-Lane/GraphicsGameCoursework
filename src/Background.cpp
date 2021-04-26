#include "Background.h"
#include "ImageLoading.h"
#include <iostream>
#include <fstream>
#include <string>
#include "nlohmann/json.hpp"
static const unsigned int s_MapWidth = 30;



Background::Background()
{
	m_TrackSheetTex = 0;
	m_MapWidth = 0;
	m_MapHeight = 0;
}

void Background::loadBackground()
{
	
	//load png image
	int imageHeight = 0;
	int imageWidth = 0;

	//create the texture on the GPU
	glGenTextures(1, &m_TrackSheetTex);
	glBindTexture(GL_TEXTURE_2D, m_TrackSheetTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  //or use GL_CLAMP
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	bool success = ImageLoading::loadImage("textures/spritesheet_tiles.png");
	if (!success) {
		std::cout << "Unable to load image file" << std::endl;
		glDeleteTextures(1, &m_TrackSheetTex);
		return;
	}

	else
	{
		std::cout << "Image loaded " << std::endl;
	}
	//create the texture on the GPU
	glGenTextures(1, &m_CheckpointTex);
	glBindTexture(GL_TEXTURE_2D, m_CheckpointTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  //or use GL_CLAMP
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	success = ImageLoading::loadImage("textures/checkpoints.png");
	if (!success) {
		std::cout << "Unable to load image file" << std::endl;
		glDeleteTextures(1, &m_CheckpointTex);
		return;
	}
	else
	{
		std::cout << "Image loaded " << std::endl;
	}

	static std::string s_MapTiles;
	static std::string s_MapEntities;
	static std::string s_npcInstructions;
	nlohmann::json level;
	std::ifstream myReadFile;
	myReadFile.open("src/level.json");
	myReadFile >> level;
	for each (std::string line in level["track"])
	{
		s_MapTiles += line;
	}
	for each (std::string line in level["entities"])
	{
		s_MapEntities += line;
	}
	for each (std::string line in level["npcInstructions"])
	{
		s_npcInstructions += line;
	}
	//yucky gross tile lookup table that i should never have made
	Tile dirt(0.0f, 0.0f, 0, 8, 128.0f, 128.0f,2304.0f, 2304.0f, m_TrackSheetTex);
	Tile track(0.0f, 0.0f, 4, 7, 128.0f, 128.0f, 2304.0f, 2304.0f, m_TrackSheetTex);
	Tile topEnd(0.0f, 0.0f, 5, 7, 128.0f, 128.0f, 2304.0f, 2304.0f, m_TrackSheetTex);
	Tile rightEnd(0.0f, 0.0f, 4, 8, 128.0f, 128.0f, 2304.0f, 2304.0f, m_TrackSheetTex);
	Tile bottomEnd(0.0f, 0.0f, 3, 7, 128.0f, 128.0f, 2304.0f, 2304.0f, m_TrackSheetTex);
	Tile leftEnd(0.0f, 0.0f, 4, 6, 128.0f, 128.0f, 2304.0f, 2304.0f, m_TrackSheetTex);

	Tile topLeft(0.0, 0.0, 5, 9, 128.0f, 128.0f, 2304.0f, 2304.0f, m_TrackSheetTex);
	Tile topRight(0.0, 0.0, 5, 10, 128.0f, 128.0f, 2304.0f, 2304.0f, m_TrackSheetTex);
	Tile bottomLeft(0.0, 0.0, 4, 9, 128.0f, 128.0f, 2304.0f, 2304.0f, m_TrackSheetTex);
	Tile bottomRight(0.0, 0.0, 4, 10, 128.0f, 128.0f, 2304.0f, 2304.0f, m_TrackSheetTex);

	Tile cornerBottomLeft(0.0, 0.0, 4, 13, 128.0f, 128.0f, 2304.0f, 2304.0f, m_TrackSheetTex);
	Tile cornerBottomRight(0.0, 0.0, 4, 12, 128.0f, 128.0f, 2304.0f, 2304.0f, m_TrackSheetTex);
	Tile cornerTopLeft(0.0, 0.0, 3, 13, 128.0f, 128.0f, 2304.0f, 2304.0f, m_TrackSheetTex);
	Tile cornerTopRight(0.0, 0.0, 3, 12, 128.0f, 128.0f, 2304.0f, 2304.0f, m_TrackSheetTex);

	Tile grassTop(0.0, 0.0, 11, 2, 128.0f, 128.0f, 2304.0f, 2304.0f, m_TrackSheetTex);
	Tile grassBottom(0.0, 0.0, 11, 6, 128.0f, 128.0f, 2304.0f, 2304.0f, m_TrackSheetTex);
	Tile grassLeft(0.0, 0.0, 12, 16, 128.0f, 128.0f, 2304.0f, 2304.0f, m_TrackSheetTex);
	Tile grassRight(0.0, 0.0, 12, 14, 128.0f, 128.0f, 2304.0f, 2304.0f, m_TrackSheetTex);

	Tile grassTopLeft(0.0, 0.0, 12, 12, 128.0f, 128.0f, 2304.0f, 2304.0f, m_TrackSheetTex);
	Tile grassBottomRight(0.0, 0.0, 11, 0, 128.0f, 128.0f, 2304.0f, 2304.0f, m_TrackSheetTex);
	Tile grassTopRight(0.0, 0.0, 12, 13, 128.0f, 128.0f, 2304.0f, 2304.0f, m_TrackSheetTex);
	Tile grassBottomLeft(0.0, 0.0, 12, 17, 128.0f, 128.0f, 2304.0f, 2304.0f, m_TrackSheetTex);

	Tile finishBottom(0.0, 0.0, 0, 2, 128.0f, 128.0f, 2304.0f, 2304.0f, m_TrackSheetTex);
	Tile finishTop(0.0, 0.0, 0, 0, 128.0f, 128.0f, 2304.0f, 2304.0f, m_TrackSheetTex);


	m_TileLookup.emplace('B', dirt);
	m_TileLookup.emplace('T', track);
	m_TileLookup.emplace('W', topEnd);
	m_TileLookup.emplace('A', leftEnd);
	m_TileLookup.emplace('S', bottomEnd);
	m_TileLookup.emplace('D', rightEnd);

	m_TileLookup.emplace('Q', topLeft);
	m_TileLookup.emplace('R', topRight);
	m_TileLookup.emplace('Z', bottomLeft);
	m_TileLookup.emplace('V', bottomRight);

	m_TileLookup.emplace('C', cornerTopLeft);
	m_TileLookup.emplace('M', cornerTopRight);
	m_TileLookup.emplace('E', cornerBottomLeft);
	m_TileLookup.emplace('F', cornerBottomRight);

	m_TileLookup.emplace('N', finishTop);
	m_TileLookup.emplace('P', finishBottom);

	m_TileLookup.emplace('I', grassTop);
	m_TileLookup.emplace('J', grassLeft);
	m_TileLookup.emplace('K', grassBottom);
	m_TileLookup.emplace('L', grassRight);

	m_TileLookup.emplace('U', grassTopLeft);
	m_TileLookup.emplace('O', grassTopRight);
	m_TileLookup.emplace('G', grassBottomLeft);
	m_TileLookup.emplace('H', grassBottomRight);

	//I hated the tile lookup table so much i made a new one for entities 
	Checkpoint checkpoint_Right_A('a', 0.0, 0.0, 1, 1, 128.0f, 128.0f, 256.0f, 256.0f, m_CheckpointTex);
	Checkpoint checkpoint_Right_B('b', 0.0, 0.0, 1, 1, 128.0f, 128.0f, 256.0f, 256.0f, m_CheckpointTex);
	Checkpoint checkpoint_Left_E('e', 0.0, 0.0, 1, 0, 128.0f, 128.0f, 256.0f, 256.0f, m_CheckpointTex);
	Checkpoint checkpoint_Left_F('f', 0.0, 0.0, 1, 0, 128.0f, 128.0f, 256.0f, 256.0f, m_CheckpointTex);

	Checkpoint checkpoint_Down_C('c', 0.0, 0.0, 0, 0, 128.0f, 128.0f, 256.0f, 256.0f, m_CheckpointTex);
	Checkpoint checkpoint_Down_D('d', 0.0, 0.0, 0, 0, 128.0f, 128.0f, 256.0f, 256.0f, m_CheckpointTex);
	Checkpoint checkpoint_Up_G('g', 0.0, 0.0, 0, 1, 128.0f, 128.0f, 256.0f, 256.0f, m_CheckpointTex);
	Checkpoint checkpoint_Up_H('h', 0.0, 0.0, 0, 1, 128.0f, 128.0f, 256.0f, 256.0f, m_CheckpointTex);

	m_EntityLookup.emplace('a', checkpoint_Right_A);
	m_EntityLookup.emplace('b', checkpoint_Right_B);
	m_EntityLookup.emplace('c', checkpoint_Down_C);
	m_EntityLookup.emplace('d', checkpoint_Down_D);
	m_EntityLookup.emplace('e', checkpoint_Left_E);
	m_EntityLookup.emplace('f', checkpoint_Left_F);
	m_EntityLookup.emplace('g', checkpoint_Up_G);
	m_EntityLookup.emplace('h', checkpoint_Up_H);

	//for loop filling the trackTiles array with track pieces, if the tile char is B then insert nothing (blank)
	m_MapWidth = 30;
	std::cout << s_MapTiles.length();
	m_MapHeight = s_MapTiles.length() / s_MapWidth;

	for (unsigned int y = 0; y < m_MapHeight; y++)
	{
		for (unsigned int x = 0; x < m_MapWidth; x++)
		{
			dirt.setXPos(x);
			dirt.setYPos(y);
			dirtTiles.push_back(dirt);

			char tileType = s_MapTiles[x + (y * m_MapWidth)]; 
			char entity = s_MapEntities[x + (y * m_MapWidth)];
			char npcInstruction = s_npcInstructions[x + (y * m_MapWidth)];

			if (m_TileLookup.find(tileType) != m_TileLookup.end())
			{
				if (entity == 'B' || entity == 'T')
				{

				}
				else {
					Checkpoint check = m_EntityLookup[entity];
					check.setXPos(x);
					check.setYPos(y);
					glm::vec2 coords = { x,y };
					checkpoints.emplace(coords, check);
				}
			}
			if (m_TileLookup.find(tileType) != m_TileLookup.end())
			{
				if (tileType == 'B')
				{
					continue;
				}
				Tile temp = m_TileLookup[tileType];
				temp.setXPos(x);
				temp.setYPos(y);
				temp.setID(tileType);
				temp.setInstruction(npcInstruction);
				glm::vec2 coords = { x,y };
				trackTiles.emplace(coords, temp);
				//std::cout << "inserting dirt";
			}
			else
			{
				std::cout << "can't find texture";
				glm::vec2 coords = { x,y };
				trackTiles.emplace(coords, dirt);
			}
		}
	}
}

unsigned int Background::GetMapWidth()
{
	return m_MapWidth;
}
unsigned int Background::GetMapHeight()
{
	return m_MapHeight;
}