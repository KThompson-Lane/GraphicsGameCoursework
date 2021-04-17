#pragma once
#include "Tile.h"
class Checkpoint: public Tile
{
public:
	Checkpoint();
	Checkpoint(char type, float x, float y, int tileOffsetX, int tileOffsetY, float tileSizeX, float tileSizeY, float sheetWidth, float sheetHeight, GLuint bgTextureName);
	char getType();
	void setType(char type);
	void Activate();
	void Reset();
	bool isActive();
private:
	char type;
	bool activated;
};

