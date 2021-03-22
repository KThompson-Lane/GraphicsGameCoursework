#pragma once

#include <string>
#include "..\glm\glm.hpp"
#include "..\glm\gtc\matrix_transform.hpp"
#include "..\gl\glew.h"

#include "..\shaders\Shader.h"
#include "OBB.h"

class Tile
{
private:
	int m_tileOffsetX;
	int m_tileOffsetY;
	float m_tileSizeX;
	float m_tileSizeY;
	float m_sheetWidth;
	float m_sheetHeight;
	GLuint m_TexName;

	unsigned int m_vaoID;		// id for Vertex Array Object
	unsigned int m_vboID[3];	// ids for Vertex Buffer Objects
	unsigned int m_NumberOfVerts;//number of vertices in the object

	//Dimensions of the sprite.
	float m_Width;
	float m_Height;

	//position of the sprite
	float m_xpos;
	float m_ypos;
	OBB obb; //obb object for oriented bounding box collision

public:
	Tile();
	Tile(int tileOffsetX, int tileOffsetY, float tileSizeX, float tileSizeY, GLuint bgTextureName);
	Tile(float x, float y, int tileOffsetX, int tileOffsetY, float tileSizeX, float tileSizeY, GLuint bgTextureName);
	Tile(float x, float y, int tileOffsetX, int tileOffsetY, float tileSizeX, float tileSizeY, float sheetWidth, float sheetHeight, GLuint bgTextureName);
	void setXPos(float x);
	void setYPos(float y);
	float GetWidth();
	float GetHeight();
	void Init(Shader& shader, float colour[3]);
	void Render(Shader& shader, glm::mat4& ModelViewMatrix, glm::mat4& ProjectionMatrix);
	OBB& GetOBB();
	bool IsInCollision(OBB& anotherOBB);
}; 


