#pragma once
#include "..\glm\glm.hpp"
#include "..\glm\gtc\matrix_transform.hpp"
#include <iostream>
#include "..\shaders\Shader.h"
#include "..\GL\freeglut.h"
#include "..\FreeImage.h"
#include "Player.h"
#include "Background.h"

#include "FreeType.h"		//include font library
using namespace freetype;


class Game
{
private:
	//booleans to handle when the arrow keys are pressed or released.
	Font lapFont;
	Font timerFont;
	float lapTime = 0.0f;
	float bestTime = 0.0f;
	bool temp = false;
	bool gamePaused = true;
	bool Left = false;
	bool Right = false;
	bool Up = false;
	bool Down = false;
	int checkpointsCompleted;
	int playerLaps = 0;
	int npcLaps = 0;
	bool NPCCrashed = false;
	bool playerCrashed = false;
	bool npcCanLap = false;
	bool playerWin = false;
	int selection = 0;
	float white[3] = { 1,1,1 };
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
	Sprite winScreen;
	Sprite loseScreen;
	Sprite pauseScreen;
	glm::mat4 ProjectionMatrix;
	glm::mat4 ViewMatrix;  // matrix for the modelling and viewing

public: 
	bool inMenu = true;
	bool gameOver = false;
	bool isPaused();
	void togglePause();
	unsigned int Width, Height;
	Game();
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
	void restartLevel();
	void restartGame();

};
