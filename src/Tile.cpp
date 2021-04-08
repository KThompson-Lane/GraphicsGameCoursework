#include "Tile.h"
#include <iostream>
Tile::Tile()
{
	this->m_tileOffsetX = 0;
	this->m_tileOffsetY = 0;
	this->m_tileSizeX = 0;
	this->m_tileSizeY = 0;
	this->m_TexName = 0;
	m_vaoID = 0;
	m_vboID[0] = 0;
	m_vboID[1] = 0;
	m_Width = 0.0f;
	m_Height = 0.0f;
	m_NumberOfVerts = 0;
	m_xpos = 0.0f;
	m_ypos = 0.0f;
	m_sheetWidth = 0.0f;
	m_sheetHeight = 0.0f;
}

Tile::Tile(int tileOffsetX, int tileOffsetY, float tileSizeX, float tileSizeY, GLuint bgTextureName)
{
	this->m_tileOffsetX = tileOffsetX;
	this->m_tileOffsetY = tileOffsetY;
	this->m_tileSizeX = tileSizeX;
	this->m_tileSizeY = tileSizeY;
	this->m_TexName = bgTextureName;
	m_vaoID = 0;
	m_vboID[0] = 0;
	m_vboID[1] = 0;
	m_Width = 0.0f;
	m_Height = 0.0f;
	m_NumberOfVerts = 0;
	m_xpos = 0.0f;
	m_ypos = 0.0f;
	m_sheetWidth = 0.0f;
	m_sheetHeight = 0.0f;
}

Tile::Tile(float x, float y, int tileOffsetX, int tileOffsetY, float tileSizeX, float tileSizeY, GLuint bgTextureName)
{
	this->m_tileOffsetX = tileOffsetX;
	this->m_tileOffsetY = tileOffsetY;
	this->m_tileSizeX = tileSizeX;
	this->m_tileSizeY = tileSizeY;
	this->m_TexName = bgTextureName;
	m_vaoID = 0;
	m_vboID[0] = 0;
	m_vboID[1] = 0;
	m_Width = 0;
	m_Height = 0;
	m_NumberOfVerts = 6;
	m_xpos = x;
	m_ypos = y;
	m_sheetWidth = 0.0f;
	m_sheetHeight = 0.0f;
}

Tile::Tile(float x, float y, int tileOffsetX, int tileOffsetY, float tileSizeX, float tileSizeY, float sheetWidth, float sheetHeight, GLuint bgTextureName)
{
	this->m_tileOffsetX = tileOffsetX;
	this->m_tileOffsetY = tileOffsetY;
	this->m_tileSizeX = tileSizeX;
	this->m_tileSizeY = tileSizeY;
	this->m_TexName = bgTextureName;
	m_vaoID = 0;
	m_vboID[0] = 0;
	m_vboID[1] = 0;
	m_Width = 20;
	m_Height = 20;
	m_NumberOfVerts = 6;
	m_xpos = x;
	m_ypos = y;
	m_sheetWidth = sheetWidth;
	m_sheetHeight = sheetHeight;
}
void Tile::setID(char id)
{
	m_id = id;
}
char Tile::getID()
{
	return m_id;
}
void Tile::setXPos(float x)
{
	m_xpos = x;
}

void Tile::setYPos(float y)
{
	m_ypos = y;
}

float Tile::getXPos()
{
	return m_xpos;
}

float Tile::getYPos()
{
	return m_ypos;
}

void Tile::setOBB()
{
	glm::vec2 bottomLeft;
	glm::vec2 bottomRight;
	glm::vec2 topLeft;
	glm::vec2 topRight;
	float halfWidth = m_Width / 2.0f;
	float halfHeight = m_Height / 2.0f;
	switch (getID())
	{
		case 'S':
			bottomLeft = { -(halfWidth), -(halfHeight) };
			bottomRight = { (halfWidth), - (halfHeight) };
			topLeft = { -(halfWidth), (-(halfHeight) + (0.15625 * 20))};
			topRight = { (halfWidth), (-(halfHeight) +(0.15625 * 20))};
			break;
		default :
			bottomLeft = { -(halfWidth), -(halfHeight) };
			bottomRight = { (halfWidth), -(halfHeight) };
			topLeft = { -(halfWidth), (halfHeight) };
			topRight = { (halfWidth), (halfHeight) };
			break;
	}

	obb.vertOriginal[0].x = bottomLeft.x; //bottom left corner of the sprite
	obb.vertOriginal[0].y = bottomLeft.y;

	obb.vertOriginal[1].x = bottomRight.x; 
	obb.vertOriginal[1].y = bottomRight.y;

	obb.vertOriginal[2].x = topRight.x;
	obb.vertOriginal[2].y = topRight.y;

	obb.vertOriginal[3].x = topLeft.x;
	obb.vertOriginal[3].y = topLeft.y;
}

float Tile::GetWidth()
{
	return m_Width;
}
float Tile::GetHeight()
{
	return m_Height;
}

void Tile::Init(Shader& shader, float colour[3])
{
	m_NumberOfVerts = 6;
	float vert[18];	// create a vertex array

	float halfWidth = m_Width / 2.0f;
	float halfHeight = m_Height / 2.0f;

	vert[0] = -halfWidth; vert[1] = halfHeight; vert[2] = 0.0; //x,y,z values for each vertex
	vert[3] = -halfWidth; vert[4] = -halfHeight; vert[5] = 0.0;
	vert[6] = halfWidth; vert[7] = -halfHeight; vert[8] = 0.0;

	vert[9] = -halfWidth; vert[10] = halfHeight; vert[11] = 0.0;
	vert[12] = halfWidth; vert[13] = halfHeight; vert[14] = 0.0;
	vert[15] = halfWidth; vert[16] = -halfHeight; vert[17] = 0.0;

	//setup obb
	setOBB();


	float tex[12];
	//U left V right (U = x V = Y)

	tex[0] = ((m_tileOffsetX * m_tileSizeX)+1)/ m_sheetWidth;			 tex[1] = (((m_tileOffsetY + 1) * m_tileSizeY)-2) / m_sheetHeight; //top left (i.e. 0,1)
	tex[2] = ((m_tileOffsetX * m_tileSizeX)+1) / m_sheetWidth;			 tex[3] = ((m_tileOffsetY * m_tileSizeY)+1) / m_sheetHeight; //bottom left(i.e. 0,1)
	tex[4] = (((m_tileOffsetX + 1) * m_tileSizeX)-2) / m_sheetWidth;	 tex[5] = ((m_tileOffsetY * m_tileSizeY)+1) / m_sheetHeight; //bottom right(i.e. 0,1)

	//tex[0] = 0.0f;	 tex[1] = 1.0; //top left
	//tex[2] = 0.0f;	 tex[3] = 0.0; //bottom left
	//tex[4] = 1.0f;	 tex[5] = 0.0; //bottom right

	tex[6] = ((m_tileOffsetX * m_tileSizeX)+1) / m_sheetWidth;		     tex[7] = (((m_tileOffsetY + 1) * m_tileSizeY)-2) / m_sheetHeight; //top left (i.e. 0,1)
	tex[8] = (((m_tileOffsetX + 1) * m_tileSizeX)-2) / m_sheetWidth;	 tex[9] = (((m_tileOffsetY + 1) * m_tileSizeY)-2) / m_sheetHeight; //top right(i.e. 0,1)
	tex[10] = (((m_tileOffsetX + 1) * m_tileSizeX)-2) / m_sheetWidth;	 tex[11] = ((m_tileOffsetY * m_tileSizeY)+1) / m_sheetHeight; //bottom right(i.e. 0,1)

	//tex[0] = (m_tileOffsetX * m_tileSizeX) / m_sheetWidth;			 tex[1] = ((m_tileOffsetY + 1) * m_tileSizeY) / m_sheetHeight; //top left (i.e. 0,1)
	//tex[2] = (m_tileOffsetX * m_tileSizeX) / m_sheetWidth;			 tex[3] = (m_tileOffsetY * m_tileSizeY) / m_sheetHeight; //bottom left(i.e. 0,1)
	//tex[4] = ((m_tileOffsetX + 1) * m_tileSizeX) / m_sheetWidth;	 tex[5] = (m_tileOffsetY * m_tileSizeY) / m_sheetHeight; //bottom right(i.e. 0,1)

	//tex[0] = 0.0f;	 tex[1] = 1.0; //top left
	//tex[2] = 0.0f;	 tex[3] = 0.0; //bottom left
	//tex[4] = 1.0f;	 tex[5] = 0.0; //bottom right

	//tex[6] = (m_tileOffsetX * m_tileSizeX) / m_sheetWidth;		     tex[7] = ((m_tileOffsetY + 1) * m_tileSizeY) / m_sheetHeight; //top left (i.e. 0,1)
	//tex[8] = ((m_tileOffsetX + 1) * m_tileSizeX) / m_sheetWidth;	 tex[9] = ((m_tileOffsetY + 1) * m_tileSizeY) / m_sheetHeight; //top right(i.e. 0,1)
	//tex[10] = ((m_tileOffsetX + 1) * m_tileSizeX) / m_sheetWidth;	 tex[11] = (m_tileOffsetY * m_tileSizeY) / m_sheetHeight; //bottom right(i.e. 0,1)

	//tex[6] = 0.0f;	 tex[7] = 1.0;	//top left
	//tex[8] = 1.0f;	 tex[9] = 1.0;	// top right
	//tex[10] = 1.0f;	 tex[11] = 0.0; //bottom right

	float col[18];	// colour array
	col[0] = colour[0]; col[1] = colour[1]; col[2] = colour[2]; //r,g,b values for each vertex
	col[3] = colour[0]; col[4] = colour[1]; col[5] = colour[2]; //r,g,b values for each vertex
	col[6] = colour[0]; col[7] = colour[1]; col[8] = colour[2]; //r,g,b values for each vertex
	col[9] = colour[0]; col[10] = colour[1]; col[11] = colour[2]; //r,g,b values for each vertex
	col[12] = colour[0]; col[13] = colour[1]; col[14] = colour[2]; //r,g,b values for each vertex
	col[15] = colour[0]; col[16] = colour[1]; col[17] = colour[2]; //r,g,b values for each vertex

	//VAO allocation
	glGenVertexArrays(1, &m_vaoID);

	// First VAO setup
	glBindVertexArray(m_vaoID);

	glGenBuffers(3, m_vboID); // we need three VBOs - one for the vertices and one for the colours
							//and an extra one for the texture coordinates

	//Lets set up the vertices.
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID[0]);

	//initialises data storage of vertex buffer object
	glBufferData(GL_ARRAY_BUFFER, m_NumberOfVerts * 3 * sizeof(GLfloat), vert, GL_STATIC_DRAW);

	//set the position - linked to the position shader input
	GLint vertexLocation = glGetAttribLocation(shader.handle(), "in_Position");
	glEnableVertexAttribArray(vertexLocation);
	glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//Now set up the colours
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID[1]);
	glBufferData(GL_ARRAY_BUFFER, m_NumberOfVerts * 3 * sizeof(GLfloat), col, GL_STATIC_DRAW);

	//set the colour - linked to the colour shader input.
	GLint colorLocation = glGetAttribLocation(shader.handle(), "in_Color");
	glEnableVertexAttribArray(colorLocation);
	//location in shader, number of componentns,  type, normalised, stride, pointer to first attribute
	glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//Now set up the texture coordinates
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID[2]);
	glBufferData(GL_ARRAY_BUFFER, m_NumberOfVerts * 3 * sizeof(GLfloat), tex, GL_STATIC_DRAW);

	//set the texture coords - linked to the texcoord shader input.
	GLint texLocation = glGetAttribLocation(shader.handle(), "in_TexCoord");
	glEnableVertexAttribArray(texLocation);
	//location in shader, number of componentns,  type, normalised, stride, pointer to first attribute
	glVertexAttribPointer(texLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);

	//good practice to bind to 0.
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

}

void Tile::Render(Shader & shader, glm::mat4 & ModelViewMatrix, glm::mat4 & ProjectionMatrix)
{
	/****UPDATE THE CORNER VALUES BASED ON TRANSFORMATION***/
	obb.transformPoints(ModelViewMatrix);
	/*******************************************************/

	glUseProgram(shader.handle());  // use the shader

	//set the DiffuseMap in GLSL to the texture unit 0.
	glUniform1i(glGetUniformLocation(shader.handle(), "DiffuseMap"), 0);

	glBindTexture(GL_TEXTURE_2D, m_TexName);

	//set the uniform for the projectionmatrix
	glUniformMatrix4fv(glGetUniformLocation(shader.handle(), "ProjectionMatrix"), 1, GL_FALSE, &ProjectionMatrix[0][0]);

	//pass the uniform for the ModelView matrix to the shader
	glUniformMatrix4fv(glGetUniformLocation(shader.handle(), "ModelViewMatrix"), 1, GL_FALSE, &ModelViewMatrix[0][0]);

	//Draw the object
	glBindVertexArray(m_vaoID);		// select first VAO
	glDrawArrays(GL_TRIANGLES, 0, m_NumberOfVerts);	// draw first object

	glBindVertexArray(0); //unbind the vertex array object
	glUseProgram(0); //turn off the current shader
}

OBB& Tile::GetOBB()
{
	return obb;
}

bool Tile::IsInCollision(OBB& anotherOBB)
{
	if (obb.SAT2D(anotherOBB))
	{
		return true;
	}
	return false;
}




