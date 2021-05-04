#include "Game.h"
#include <irrKlang/include/irrKlang.h>




irrklang::ISoundEngine* SoundEngine = irrklang::createIrrKlangDevice();
irrklang::ISound* EngineNoise;

Game::Game()
{
	NPC.SetTopSpeed(0.8f);
	player.SetTopSpeed(0.8f);
	Width = 1920;
	Height = 1080;
}

Game::Game(unsigned int width, unsigned int height)
{
	NPC.SetTopSpeed(0.8f);
	player.SetTopSpeed(0.8f);
	Width = width;
	Height = height;
}

Game::~Game()
{
}

void Game::Init()
{
	glClearColor((39.0f/255.0f),(174.0f/255.0f), (96.0f/255.0f), 0.0);	//sets the clear colour to whatever RGB values passed in, normalized between 0 and 1 

	///This part commented is to scale the width of the sprite to match the dimensions of the car.png image.
	player.SetWidth(100.0f * (264.0f / 500.0f));
	player.SetHeight(100.0f);
	player.Init(shader, white, "textures/car.png");

	NPC.SetWidth(100.0f * (264.0f / 500.0f));
	NPC.SetHeight(100.0f);
	NPC.Init(shader, white, "textures/blueCar.png");

	NPC.SetXpos(200 * 4);
	NPC.IncPos(0, 200 * 2);
	NPC.SetRot(1.5708f);

	player.SetXpos(200 * 4);
	player.IncPos(0,200 * 3);
	player.SetRot(1.5708f);

	pauseScreen.SetWidth(500.0f * (768.0f / 512.0f));
	pauseScreen.SetHeight(500.0f);
	pauseScreen.Init(shader, white, "textures/PauseScreen.png");

	winScreen.SetWidth(500.0f * (768.0f / 512.0f));
	winScreen.SetHeight(500.0f);
	winScreen.Init(shader, white, "textures/WinScreen.png");

	loseScreen.SetWidth(500.0f * (768.0f / 512.0f));
	loseScreen.SetHeight(500.0f);
	loseScreen.Init(shader, white, "textures/LoseScreen.png");

	bg.loadBackground(selection + 1);
	for (Tile& dirtTile : bg.dirtTiles )
	{
		dirtTile.Init(shader, white);
	}
	for (auto& it : bg.trackTiles)
	{
		it.second.Init(shader, white);
	}
	for (auto& it : bg.checkpoints)
	{
		it.second.Init(shader, white);
	}

	float black[3] = { 0,0,0 };
	lapFont.init("fonts/BAUHS93.TTF", 75, black);
	timerFont.init("fonts/BAUHS93.TTF", 40, black);
	SoundEngine->setSoundVolume(0.5f);
	EngineNoise = SoundEngine->play2D("music/engineSound.wav", true, true);
	EngineNoise->setVolume(0.1f);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

bool Game::isPaused()
{
	return gamePaused;
}

void Game::togglePause()
{
	gamePaused = !gamePaused;
}

void Game::Update(float dt)
{
	if (!gamePaused && !gameOver)
	{
		float xDist = 0.0f;
		float yDist = 0.0f;
		float temp = 0.0f;
		glm::vec2 coords;
		

		if (player.IsInCollision(NPC.GetOBB()))
		{
			player.SetSpeed(-player.GetSpeed() / 2);
			player.IncPos(((player.GetSpeed() * dt * 10) * sinf(player.GetRot())), ((player.GetSpeed() * dt * 10) * cosf(player.GetRot()))); //same as above
			NPC.SetSpeed(-NPC.GetSpeed() / 2);
			NPC.IncPos(((NPC.GetSpeed() * dt * 10) * sinf(NPC.GetRot())), ((NPC.GetSpeed() * dt * 10) * cosf(NPC.GetRot()))); //same as above
			if (playerCrashed == false)
			{
				playerCrashed = true;
				player.Init(shader, white, "textures/car_broken.png");
				player.SetTopSpeed(player.GetTopSpeed() * 0.85);

			}
			if (NPCCrashed == false)
			{
				NPCCrashed = true;
				NPC.Init(shader, white, "textures/blueCar_broken.png");
				NPC.SetTopSpeed(NPC.GetTopSpeed() * 0.85);
			}
		}
		switch (AICurrentTile().getID())
		{
		case 'P':
		case 'N':
			CompleteLapNPC();
			break;
		}
		switch (PlayersCurrentTile().getID())
		{
		case 'P':
		case 'N':
			CompleteLapPlayer();
		case 'S':
		case 'W':
		case 'D':
		case 'A':
			if (PlayersCurrentTile().IsInCollision(player.GetOBB()))
			{
				if (playerCrashed == false)
				{
					playerCrashed = true;
					player.Init(shader, white, "textures/car_broken.png");
					player.SetTopSpeed(player.GetTopSpeed() * 0.85);

				}
				player.SetSpeed(-player.GetSpeed());
				player.IncPos(((player.GetSpeed() * dt * 2) * sinf(player.GetRot())), ((player.GetSpeed() * dt * 2) * cosf(player.GetRot()))); //same as above
			}
			break;
		case 'R':
			//check collision
			xDist = (((PlayersCurrentTile().getXPos()) - 100) - (player.GetXPos()));
			yDist = ((player.GetYPos()) - ((PlayersCurrentTile().getYPos()) + 100));
			temp = sqrt((xDist * xDist) + (yDist * yDist));
			if (temp > 150.0f)
			{
				if (playerCrashed == false)
				{
					playerCrashed = true;
					player.Init(shader, white, "textures/car_broken.png");
					player.SetTopSpeed(player.GetTopSpeed() * 0.85);

				}
				player.SetSpeed(-player.GetSpeed());
				player.IncPos((((player.GetSpeed()) * dt) * sinf(player.GetRot())), (((player.GetSpeed()) * dt) * cosf(player.GetRot())));
			}
			break;
		case 'V':
			//check collision
			xDist = (((PlayersCurrentTile().getXPos()) - 100) - (player.GetXPos()));
			yDist = ((player.GetYPos()) - ((PlayersCurrentTile().getYPos()) - 100));
			temp = sqrt((xDist * xDist) + (yDist * yDist));
			if (temp > 150.0f)
			{
				if (playerCrashed == false)
				{
					playerCrashed = true;
					player.Init(shader, white, "textures/car_broken.png");
					player.SetTopSpeed(player.GetTopSpeed() * 0.85);

				}
				player.SetSpeed(-player.GetSpeed());
				player.IncPos((((player.GetSpeed()) * dt) * sinf(player.GetRot())), (((player.GetSpeed()) * dt) * cosf(player.GetRot())));
			}
			break;

		case 'Z':
			//check collision
			xDist = (((PlayersCurrentTile().getXPos()) + 100) - (player.GetXPos()));
			yDist = ((player.GetYPos()) - ((PlayersCurrentTile().getYPos()) - 100));
			temp = sqrt((xDist * xDist) + (yDist * yDist));
			if (temp > 150.0f)
			{
				if (playerCrashed == false)
				{
					playerCrashed = true;
					player.Init(shader, white, "textures/car_broken.png");
					player.SetTopSpeed(player.GetTopSpeed() * 0.85);

				}
				player.SetSpeed(-player.GetSpeed());
				player.IncPos((((player.GetSpeed()) * dt) * sinf(player.GetRot())), (((player.GetSpeed()) * dt) * cosf(player.GetRot())));
			}
			break;

		case 'Q':
			//check collision
			xDist = (((PlayersCurrentTile().getXPos()) + 100) - (player.GetXPos()));
			yDist = ((player.GetYPos()) - ((PlayersCurrentTile().getYPos()) + 100));
			temp = sqrt((xDist * xDist) + (yDist * yDist));
			if (temp > 150.0f)
			{
				if (playerCrashed == false)
				{
					playerCrashed = true;
					player.Init(shader, white, "textures/car_broken.png");
					player.SetTopSpeed(player.GetTopSpeed() * 0.85);
				}
				player.SetSpeed(-player.GetSpeed());
				player.IncPos((((player.GetSpeed()) * dt) * sinf(player.GetRot())), (((player.GetSpeed()) * dt) * cosf(player.GetRot())));
			}
			break;
		}
		glm::vec2 tilePos = { std::round(player.GetXPos() / 200), std::round((player.GetYPos()) / 200) };
		switch (bg.checkpoints[tilePos].getType())
		{
		case 'e':
		case 'a':
			//activate this tile and the one below it
			if (bg.checkpoints[tilePos].isActive())
			{
				break;
			}
			bg.checkpoints[tilePos].Activate();
			coords.x = tilePos.x;
			coords.y = tilePos.y + 1;
			bg.checkpoints[coords].Activate();
			checkpointsCompleted += 2;
			break;
		case 'f':
		case 'b':
			//activate this tile and the one above it
			if (bg.checkpoints[tilePos].isActive())
			{
				break;
			}
			bg.checkpoints[tilePos].Activate();
			coords.x = tilePos.x;
			coords.y = tilePos.y - 1;
			bg.checkpoints[coords].Activate();
			checkpointsCompleted += 2;
			break;
		case 'g':
		case 'c':
			//activate this tile and the one right of it
			if (bg.checkpoints[tilePos].isActive())
			{
				break;
			}
			bg.checkpoints[tilePos].Activate();
			coords.x = tilePos.x + 1;
			coords.y = tilePos.y;
			bg.checkpoints[coords].Activate();
			checkpointsCompleted += 2;
			break;
		case 'h':
		case 'd':
			//activate this tile and the one left of it
			if (bg.checkpoints[tilePos].isActive())
			{
				break;
			}
			bg.checkpoints[tilePos].Activate();
			coords.x = tilePos.x - 1;
			coords.y = tilePos.y;
			bg.checkpoints[coords].Activate();
			checkpointsCompleted += 2;
			break;
		}
		AIMove(dt);
		if (EngineNoise->getIsPaused() && player.GetSpeed() != 0)
		{
			EngineNoise->setIsPaused(false);
		}
		else if (!EngineNoise->getIsPaused() && player.GetSpeed() > 0)
		{
			EngineNoise->setPlaybackSpeed(1.0f * pow(1 + player.GetSpeed()/2, 2));
		}
		else if (!EngineNoise->getIsPaused() && player.GetSpeed() < 0)
		{
			EngineNoise->setPlaybackSpeed(1.0f * pow(1 + -player.GetSpeed()/2, 2));
		}
		else
		{
			EngineNoise->setIsPaused(true);
		}
		//dt is time in miliseconds
		lapTime += dt/1000;
	}
	else 
	{
		EngineNoise->setIsPaused(true);
	}
}

void Game::CompleteLapNPC()
{
	if (npcCanLap)
	{
		npcCanLap = false;
		npcLaps++;
		if (npcLaps == 3)
		{
			gameOver = true;
		}
	}
}

void Game::CompleteLapPlayer()
{
	if (checkpointsCompleted == bg.GetCheckpoints())
	{
		for (auto& it : bg.checkpoints)
		{
			it.second.Reset();
		}
		checkpointsCompleted = 0;

		if (playerLaps+1 == 3)
		{
			gameOver = true;
			playerWin = true;
		}
		else
		{
			playerLaps++;
		}
		if (bestTime > lapTime || bestTime == 0.0f)
		{
			bestTime = lapTime;
		}
		lapTime = 0.0f;
	}
}

Tile& Game::AICurrentTile()
{
	glm::vec2 tilePos = { std::round(NPC.GetXPos() / 200), std::round((NPC.GetYPos()) / 200) };
	return bg.trackTiles[tilePos];
}

Tile& Game::PlayersCurrentTile()
{
	glm::vec2 tilePos = { std::round(player.GetXPos() / 200), std::round((player.GetYPos()) / 200) };
	return bg.trackTiles[tilePos];
}

void Game::AIMove(float dt)
{
	float rotInDeg = NPC.GetRot() * 180 / 3.14;
	switch (AICurrentTile().getInstruction())
	{
		case 'N':
			//rotate until facing 180 whilst moving forward;
			if (rotInDeg > 180.0f)
			{
				//turn right
				NPC.IncRot(-0.005f * dt * NPC.GetSpeed());
			}
			else if (rotInDeg < 180.0f)
			{
				//turn left
				NPC.IncRot(0.005f * dt * NPC.GetSpeed());
			}
			if (NPC.GetSpeed() <= NPC.GetTopSpeed())
			{
				NPC.IncSpeed(0.0007f * dt);
			}
			break;
		case 'S':
			//rotate until facing 0 degrees whilst moving forward;
			if (rotInDeg > 180.0f && rotInDeg < 360.0f)
			{
				//turn left
				NPC.IncRot(0.005f * dt * NPC.GetSpeed());
			}
			if (rotInDeg > 0.0f && rotInDeg <= 180.0f)
			{
				//turn right
				NPC.IncRot(-0.005f * dt * NPC.GetSpeed());

			}
			if (NPC.GetSpeed() <= NPC.GetTopSpeed())
			{
				NPC.IncSpeed(0.0007f * dt);
			}
			break;
		case 'K':
			npcCanLap = true;
		case 'E':
			//rotate until facing 180 whilst moving forward;
			if (rotInDeg > 90.0f)
			{
				//turn right
				NPC.IncRot(-0.005f * dt * NPC.GetSpeed());
			}
			else if (rotInDeg < 90.0f)
			{
				//turn left
				NPC.IncRot(0.005f * dt * NPC.GetSpeed());
			}
			if (NPC.GetSpeed() <= NPC.GetTopSpeed())
			{
				NPC.IncSpeed(0.0007f * dt);
			}
			break;
		case 'W':
			//rotate until facing 180 whilst moving forward;
			if (rotInDeg > 270.0f)
			{
				//turn right
				NPC.IncRot(-0.005f * dt * NPC.GetSpeed());
			}
			else if (rotInDeg < 270.0f)
			{
				//turn left
				NPC.IncRot(0.005f * dt * NPC.GetSpeed());
			}
			if (NPC.GetSpeed() <= NPC.GetTopSpeed())
			{
				NPC.IncSpeed(0.0007f * dt);
			}
			break;
		case 'F':
			if (NPC.GetSpeed() <= NPC.GetTopSpeed())
			{
				NPC.IncSpeed(0.0007f * dt);
			}
			//move forward
			break;
		case 'R':
			//rotate right while moving forward at half speed;
			if (NPC.GetSpeed() <= NPC.GetTopSpeed() / 1.5)
			{
				NPC.IncSpeed(0.0007f * dt);
			}
			else
			{
				NPC.IncSpeed(-0.001f * dt);
			}
			if (NPC.GetSpeed() > 0.0f)
			{
				NPC.IncRot(-0.005f * dt * NPC.GetSpeed());
			}
			break;
		case'L':
			//rotate left while moving forward at half speed;
			if (NPC.GetSpeed() <= NPC.GetTopSpeed() / 1.5)
			{
				NPC.IncSpeed(0.0007f * dt);
			}
			else
			{
				NPC.IncSpeed(-0.001f * dt);
			}
			if (NPC.GetSpeed() > 0.0f)
			{
				NPC.IncRot(0.005f * dt * NPC.GetSpeed());
			}
			break;

		default:
			break;

	}
	NPC.IncPos(((NPC.GetSpeed() * dt) * sinf(NPC.GetRot())), ((NPC.GetSpeed() * dt) * cosf(NPC.GetRot()))); //same as above
}

void Game::ProcessInput(float dt)
{
	if (!gamePaused && !gameOver)
	{
		if (Left && (player.GetSpeed() != 0.0f))
		{
			if (player.GetSpeed() > 0)
			{
				player.IncRot(0.005f * dt * player.GetSpeed());
			}
			else
			{
				player.IncRot(-(0.005f * dt * player.GetSpeed()));
			}
		}
		if (Right && (player.GetSpeed() != 0.0f))
		{
			if (player.GetSpeed() > 0)
			{
				player.IncRot(-0.005f * dt * player.GetSpeed());
			}
			else
			{
				player.IncRot(0.005f * dt * player.GetSpeed());
			}
		}
		if (Up && !Down)
		{
			if (player.GetSpeed() < 0)
			{
				player.IncSpeed(0.005f * dt);
			}
			if (player.GetSpeed() <= player.GetTopSpeed())
			{
				player.IncSpeed(0.0007f * dt);
			}
		}
		if (Down && !Up)
		{
			if (player.GetSpeed() > 0)
			{
				player.IncSpeed(-0.005f * dt);
			}
			if (player.GetSpeed() >= -((player.GetTopSpeed() - (player.GetTopSpeed() / 10))))
			{
				player.IncSpeed(-0.0007f * dt);
			}
		}
		if (!Up && !Down)
		{

			if (player.GetSpeed() < 0)
			{
				player.IncSpeed(0.0005f * dt);
			}
			else if (player.GetSpeed() > 0)
			{
				player.IncSpeed(-0.0005 * dt);
			}
			if ((player.GetSpeed() > -0.005 && player.GetSpeed() < 0.0f) || (player.GetSpeed() < 0.005 && player.GetSpeed() > 0.0f))
			{
				player.SetSpeed(0);
			}
		}
		//std::cout << player.GetSpeed();
		player.IncPos(((player.GetSpeed() * dt) * sinf(player.GetRot())), ((player.GetSpeed() * dt) * cosf(player.GetRot()))); //same as above

	}
}

void Game::Render()
{
	//clear the colour and depth buffers
	glClear(GL_COLOR_BUFFER_BIT);
	
	//make work
	glm::mat4 cameraMatrix = ProjectionMatrix;
	cameraMatrix = glm::translate(cameraMatrix, glm::vec3(-player.GetXPos(), player.GetYPos() - bg.GetMapHeight(), 0.0));
	ViewMatrix = glm::translate(glm::mat4(1.0), glm::vec3(0.0, 0.0, 0.0));

	glm::mat4 ModelViewMatrix = glm::translate(getViewMatrix(), glm::vec3(player.GetXPos(), player.GetYPos(), 0.0));
	glm::mat4 TileViewMatrix;
	ModelViewMatrix = glm::mat4(1.0), glm::vec3(0.0, 0.0, 0.0);
	for (Tile& dirtTile : bg.dirtTiles)
	{
		TileViewMatrix = glm::translate(glm::mat4(1.0), glm::vec3(dirtTile.getXPos(), (bg.GetMapHeight() - (dirtTile.getYPos())), 0.0));
		dirtTile.Render(shader, TileViewMatrix, cameraMatrix);
	}
	glEnable(GL_BLEND);
	for (auto& it : bg.trackTiles)
	{
		TileViewMatrix = glm::translate(glm::mat4(1.0), glm::vec3(it.second.getXPos(), (bg.GetMapHeight() - (it.second.getYPos())), 0.0));
		it.second.Render(shader, TileViewMatrix, cameraMatrix);
	}
	for (auto& it : bg.checkpoints)
	{
		if (it.second.isActive())
		{
			continue;
		}
		TileViewMatrix = glm::translate(glm::mat4(1.0), glm::vec3(it.second.getXPos(), (bg.GetMapHeight() - (it.second.getYPos() )), 0.0));
		it.second.Render(shader, TileViewMatrix, cameraMatrix);
	}

	//player rendering code
	ViewMatrix = glm::translate(glm::mat4(1.0), glm::vec3(0.0, 0.0, 0.0));
	ModelViewMatrix = glm::translate(ViewMatrix, glm::vec3(player.GetXPos(), bg.GetMapHeight() -player.GetYPos(), 0.0));
	ModelViewMatrix = glm::rotate(ModelViewMatrix, player.GetRot(), glm::vec3(0.0, 0.0, 1.0));
	player.Render(shader, ModelViewMatrix, cameraMatrix);

	//npc rendering code
	ViewMatrix = glm::translate(glm::mat4(1.0), glm::vec3(0.0, 0.0, 0.0));
	ModelViewMatrix = glm::translate(ViewMatrix, glm::vec3(NPC.GetXPos(), bg.GetMapWidth() -NPC.GetYPos(), 0.0));
	ModelViewMatrix = glm::rotate(ModelViewMatrix, NPC.GetRot(), glm::vec3(0.0, 0.0, 1.0));
	NPC.Render(shader, ModelViewMatrix, cameraMatrix);

	ViewMatrix = glm::translate(glm::mat4(1.0), glm::vec3(0.0, 0.0, 0.0));


	if (gamePaused)
	{
		pauseScreen.Render(shader, ViewMatrix, ProjectionMatrix);
	}

	if (gameOver && playerWin)
	{
		winScreen.Render(shader, ViewMatrix, ProjectionMatrix);
	}
	else if (gameOver && !playerWin)
	{
		loseScreen.Render(shader, ViewMatrix, ProjectionMatrix);
	}
	//lap rendering
	ViewMatrix = glm::translate(glm::mat4(1.0), glm::vec3(0.0, 300.0, 0.0));
	print(ProjectionMatrix, lapFont, -170, 280, "Lap :%d/%d", playerLaps + 1, 3);

	print(ProjectionMatrix, timerFont, +220, 300, "Best Time : %4.2f", bestTime);
	print(ProjectionMatrix, timerFont, +220, 250, "Time : %4.2f", lapTime);
	print(ProjectionMatrix, timerFont, -640, 300, "Checkpoints: %d/%d", checkpointsCompleted / 2, (bg.GetCheckpoints() / 2));
	glDisable(GL_BLEND);
	glutSwapBuffers();
}

void Game::setLeft(bool left)
{
	Left = left;
}

void Game::setRight(bool right)
{
	Right = right;
}

void Game::setUp(bool up)
{
	Up = up;
}

void Game::setDown(bool down)
{
	Down = down;
}

void Game::setProjMatrix(glm::mat4 projMatrix)
{
	ProjectionMatrix = projMatrix;
}

glm::mat4 Game::getViewMatrix()
{
	return ViewMatrix;
}

void Game::RenderMenu()
{
	//clear the colour and depth buffers
	glClear(GL_COLOR_BUFFER_BIT);

	ViewMatrix = glm::translate(glm::mat4(1.0), glm::vec3(0.0, 200.0, 0.0));
	glEnable(GL_BLEND);
	titleScreen.Render(shader, ViewMatrix, ProjectionMatrix);

	ViewMatrix = glm::translate(glm::mat4(1.0), glm::vec3(-300.0, -50.0, 0.0));
	level1Label.Render(shader, ViewMatrix, ProjectionMatrix);

	ViewMatrix = glm::translate(ViewMatrix, glm::vec3(300.0, 0.0, 0.0));
	level2Label.Render(shader, ViewMatrix, ProjectionMatrix);

	ViewMatrix = glm::translate(ViewMatrix, glm::vec3(300.0, 0.0, 0.0));
	level3Label.Render(shader, ViewMatrix, ProjectionMatrix);

	glDisable(GL_BLEND);

	ViewMatrix = glm::translate(glm::mat4(1.0), glm::vec3(-300.0, -200.0, 0.0));

	level1.Render(shader, ViewMatrix, ProjectionMatrix);

	ViewMatrix = glm::translate(ViewMatrix, glm::vec3(300.0, 0.0, 0.0));

	level2.Render(shader, ViewMatrix, ProjectionMatrix);

	ViewMatrix = glm::translate(ViewMatrix, glm::vec3(300.0, 0.0, 0.0));

	level3.Render(shader, ViewMatrix, ProjectionMatrix);

	glutSwapBuffers();
}

void Game::InitMenu()
{
	FreeImage_Initialise();

	//Load the GLSL program 
	if (!shader.load("Basic", "./glslfiles/basicTexture.vert", "./glslfiles/basicTexture.frag"))
	{
		std::cout << "failed to load shader" << std::endl;
	}
	glClearColor((0.0f / 255.0f), (0.0f / 255.0f), (0.0f / 255.0f), 0.0);				//sets the clear colour to whatever RGB values passed in, normalized between 0 and 1 
	
	
	titleScreen.SetWidth(300.0f * (1024 / 512));
	titleScreen.SetHeight(300.0f);

	level1Label.SetWidth(50.0f * (320 / 64));
	level1Label.SetHeight(50.0f);

	level2Label.SetWidth(50.0f * (320 / 64));
	level2Label.SetHeight(50.0f);

	level3Label.SetWidth(50.0f * (320 / 64));
	level3Label.SetHeight(50.0f);

	level1.SetWidth(250.0f);
	level1.SetHeight(250.0f);
	std::cout << "init";
	level2.SetWidth(250.0f);
	level2.SetHeight(250.0f);
	level3.SetWidth(250.0f);
	level3.SetHeight(250.0f);

	titleScreen.Init(shader, white, "textures/TitleScreen.png");
	level1Label.Init(shader, white, "textures/level_1_label.png");
	level2Label.Init(shader, white, "textures/level_2_label.png");
	level3Label.Init(shader, white, "textures/level_3_label.png");

	level1.Init(shader, white, "textures/level_1.png");
	level2.Init(shader, white, "textures/level_2_deselected.png");
	level3.Init(shader, white, "textures/level_3_deselected.png");
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void Game::MainMenu()
{
	switch (selection)
	{
	case 0:
		level1.Init(shader, white, "textures/level_1.png");
		level2.Init(shader, white, "textures/level_2_deselected.png");
		level3.Init(shader, white, "textures/level_3_deselected.png");
		break;
	case 1:
		level1.Init(shader, white, "textures/level_1_deselected.png");
		level2.Init(shader, white, "textures/level_2.png");
		level3.Init(shader, white, "textures/level_3_deselected.png");
		break;
	case 2:
		level1.Init(shader, white, "textures/level_1_deselected.png");
		level2.Init(shader, white, "textures/level_2_deselected.png");
		level3.Init(shader, white, "textures/level_3.png");
		break;
	default:
		level1.Init(shader, white, "textures/level_1.png");
		level2.Init(shader, white, "textures/level_2_deselected.png");
		level3.Init(shader, white, "textures/level_3_deselected.png");
		break;
	}
}

void Game::setSelection(int selection)
{
	this->selection = selection;
}

int Game::getSelection()
{
	return this->selection;
}

void Game::restartLevel()
{
	NPC.SetYpos(0.0f);
	NPC.SetXpos(200 * 4);
	NPC.IncPos(0, 200 * 2);
	NPC.SetRot(1.5708f);
	NPC.SetTopSpeed(0.8f);

	NPCCrashed = false;
	NPC.Init(shader, white, "textures/blueCar.png");

	player.SetYpos(0.0f);
	player.SetXpos(200 * 4);
	player.IncPos(0, 200 * 3);
	player.SetRot(1.5708f);
	player.SetTopSpeed(0.8f);

	playerCrashed = false;
	player.Init(shader, white, "textures/car.png");
	lapTime = 0.0f;
	playerLaps = 0;
	npcLaps = 0;
	npcCanLap = false;
	playerWin = false;
	player.SetSpeed(0.0f);
	NPC.SetSpeed(0.0f);
	togglePause();
	gameOver = false;
}