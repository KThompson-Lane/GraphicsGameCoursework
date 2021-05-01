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
	bool temp = false;
	bool gamePaused = true;
	bool Left = false;
	bool Right = false;
	bool Up = false;
	bool Down = false;
	int checkpointsCompleted;
	int playerLaps = 0;
	int npcLaps = 0;
	bool npcCanLap = false;
	bool playerWin = false;
	int selection = 0;
	float red[3] = { 1,0,0 };
	Shader shader;
	Player player;
	Player NPC;
	Background bg;
	Sprite titleScreen;
	Sprite level1Label;
	Sprite level2Label;
	Sprite level3Label;
	Sprite level1;
	Sprite level2;
	Sprite level3;
	Sprite lap1;
	Sprite lap2;
	Sprite lap3;
	Sprite winScreen;
	Sprite loseScreen;
	Sprite pauseScreen;
	glm::mat4 ProjectionMatrix;
	glm::mat4 ViewMatrix;  // matrix for the modelling and viewing

public: 
	bool inMenu = true;
	bool gameOver = false;
	void togglePause();
	unsigned int Width, Height;
	Game(unsigned int width, unsigned int height);
	~Game();
	void Init();
	void ProcessInput(float dt);
	void Update(float dt);
	void CompleteLapNPC();
	void CompleteLapPlayer();
	Tile& AICurrentTile();
	Tile& PlayersCurrentTile();
	void AIMove(float dt);
	void Render();
	void setLeft(bool left);
	void setRight(bool right);
	void setUp(bool up);
	void setDown(bool down);
	void setProjMatrix(glm::mat4 projMatrix);
	glm::mat4 getViewMatrix();
	void RenderMenu();
	void InitMenu();
	void MainMenu();
	void setSelection(int selection);
	int getSelection();
	void restartGame();

};
