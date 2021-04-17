#pragma once
#include "..\glm\glm.hpp"
#include "..\glm\gtc\matrix_transform.hpp"
#include <iostream>
#include "..\shaders\Shader.h"
#include "..\GL\freeglut.h"
#include "..\FreeImage.h"
#include "Player.h"
#include "Background.h"
class Game
{
private:
	//booleans to handle when the arrow keys are pressed or released.
	bool Left = false;
	bool Right = false;
	bool Up = false;
	bool Down = false;
	bool gameOver = false;
	int checkpointsCompleted;
	Shader shader;
	Player player;
	Background bg;
	Sprite myOtherSquare;
	glm::mat4 ProjectionMatrix;
	glm::mat4 ViewMatrix;  // matrix for the modelling and viewing
public: 
	unsigned int Width, Height;
	Game(unsigned int width, unsigned int height);
	~Game();
	void Init();
	void ProcessInput(float dt);
	void Update(float dt);
	void CompleteLap();
	Tile& PlayersCurrentTile();
	void Render();
	void setLeft(bool left);
	void setRight(bool right);
	void setUp(bool up);
	void setDown(bool down);
	void setProjMatrix(glm::mat4 projMatrix);
	glm::mat4 getViewMatrix();
};
