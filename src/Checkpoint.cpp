#include "Checkpoint.h"

Checkpoint::Checkpoint()
	: Tile()
{
	this->type = 'T';
	this->activated = false;
}

Checkpoint::Checkpoint(char type, float x, float y, int tileOffsetX, int tileOffsetY, float tileSizeX, float tileSizeY, float sheetWidth, float sheetHeight, GLuint bgTextureName)
	: Tile(x, y, tileOffsetX, tileOffsetY, tileSizeX, tileSizeY, sheetWidth, sheetHeight, bgTextureName)
{
	this->type = type;
	activated = false;
}

char Checkpoint::getType()
{
	return type;
}

void Checkpoint::setType(char type)
{
	this->type = type;
}

void Checkpoint::Activate()
{
	activated = true;
}

void Checkpoint::Reset()
{
	activated = false;
}

bool Checkpoint::isActive()
{
	return activated;
}
