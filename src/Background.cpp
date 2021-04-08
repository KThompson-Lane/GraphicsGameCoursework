#include "Background.h"
#include "ImageLoading.h"
#include <iostream>
#include <fstream>
#include <string>
static const unsigned int s_MapWidth = 30;



Background::Background()
{
	m_TexName = 0;
	m_MapWidth = 0;
	m_MapHeight = 0;
}

void Background::loadBackground(std::string file, float tileSizeX, float tileSizeY)
{

	//load png image
	int imageHeight = 0;
	int imageWidth = 0;

	//create the texture on the GPU
	glGenTextures(1, &m_TexName);
	glBindTexture(GL_TEXTURE_2D, m_TexName);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  //or use GL_CLAMP
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	bool success = ImageLoading::loadImage(file);
	if (!success) {
		std::cout << "Unable to load image file" << std::endl;
		glDeleteTextures(1, &m_TexName);
		return;
	}
	else
	{
		std::cout << "Image loaded " << std::endl;
	}
	static std::string s_MapTiles;
	std::string line;
	std::ifstream myReadFile;

	myReadFile.open("src/track.txt");
	if (myReadFile.is_open())
	{
		while (std::getline(myReadFile, line))
		{
			s_MapTiles += line;
		}
	}
	else
	{
		std::cout << "CAN'T OPEN FILE";
	}
	Tile dirt(0.0f, 0.0f, 0, 8, 128.0f, 128.0f,2304.0f, 2304.0f, m_TexName);
	Tile track(0.0f, 0.0f, 4, 7, 128.0f, 128.0f, 2304.0f, 2304.0f, m_TexName);
	Tile topEnd(0.0f, 0.0f, 5, 7, 128.0f, 128.0f, 2304.0f, 2304.0f, m_TexName);
	Tile rightEnd(0.0f, 0.0f, 4, 8, 128.0f, 128.0f, 2304.0f, 2304.0f, m_TexName);
	Tile bottomEnd(0.0f, 0.0f, 3, 7, 128.0f, 128.0f, 2304.0f, 2304.0f, m_TexName);
	Tile leftEnd(0.0f, 0.0f, 4, 6, 128.0f, 128.0f, 2304.0f, 2304.0f, m_TexName);

	Tile topLeft(0.0, 0.0, 5, 9, 128.0f, 128.0f, 2304.0f, 2304.0f, m_TexName);
	Tile topRight(0.0, 0.0, 5, 10, 128.0f, 128.0f, 2304.0f, 2304.0f, m_TexName);
	Tile bottomLeft(0.0, 0.0, 4, 9, 128.0f, 128.0f, 2304.0f, 2304.0f, m_TexName);
	Tile bottomRight(0.0, 0.0, 4, 10, 128.0f, 128.0f, 2304.0f, 2304.0f, m_TexName);


	Tile grassTop(0.0, 0.0, 11, 2, 128.0f, 128.0f, 2304.0f, 2304.0f, m_TexName);
	Tile grassBottom(0.0, 0.0, 11, 6, 128.0f, 128.0f, 2304.0f, 2304.0f, m_TexName);
	Tile grassLeft(0.0, 0.0, 12, 16, 128.0f, 128.0f, 2304.0f, 2304.0f, m_TexName);
	Tile grassRight(0.0, 0.0, 12, 14, 128.0f, 128.0f, 2304.0f, 2304.0f, m_TexName);

	Tile grassTopLeft(0.0, 0.0, 12, 12, 128.0f, 128.0f, 2304.0f, 2304.0f, m_TexName);
	Tile grassBottomRight(0.0, 0.0, 11, 0, 128.0f, 128.0f, 2304.0f, 2304.0f, m_TexName);
	Tile grassTopRight(0.0, 0.0, 12, 13, 128.0f, 128.0f, 2304.0f, 2304.0f, m_TexName);
	Tile grassBottomLeft(0.0, 0.0, 12, 17, 128.0f, 128.0f, 2304.0f, 2304.0f, m_TexName);

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


	m_TileLookup.emplace('I', grassTop);
	m_TileLookup.emplace('J', grassLeft);
	m_TileLookup.emplace('K', grassBottom);
	m_TileLookup.emplace('L', grassRight);

	m_TileLookup.emplace('U', grassTopLeft);
	m_TileLookup.emplace('O', grassTopRight);
	m_TileLookup.emplace('G', grassBottomLeft);
	m_TileLookup.emplace('H', grassBottomRight);


	//for loop filling the dirtTiles array with dirt




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
				trackTiles.push_back(temp);
				//std::cout << "inserting dirt";
			}
			else
			{
				std::cout << "can't find texture";
				trackTiles.push_back(dirt);
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