#include "Game.h"
#include <irrKlang/include/irrKlang.h>



irrklang::ISoundEngine* SoundEngine = irrklang::createIrrKlangDevice();
int count = 0;

Game::Game(unsigned int width, unsigned int height)
{
	NPC.SetTopSpeed(0.08f);
	player.SetTopSpeed(0.08f);
	Width = width;
	Height = height;
}

Game::~Game()
{

}

void Game::Init()
{
	FreeImage_Initialise();
	
	glClearColor((39.0f/255.0f),(174.0f/255.0f), (96.0f/255.0f), 0.0);						//sets the clear colour to whatever RGB values passed in, normalized between 0 and 1 

	//Load the GLSL program 
	if (!shader.load("Basic", "./glslfiles/basicTexture.vert", "./glslfiles/basicTexture.frag"))
	{
		std::cout << "failed to load shader" << std::endl;
	}

	///This part commented is to scale the width of the sprite to match the dimensions of the car.png image.
	player.SetWidth(10.0f * (264.0f / 500.0f));
	player.SetHeight(10.0f);
	float red[3] = { 1,0,0 };
	player.Init(shader, red, "textures/car.png");

	NPC.SetWidth(10.0f * (264.0f / 500.0f));
	NPC.SetHeight(10.0f);
	NPC.Init(shader, red, "textures/blueCar.png");

	NPC.SetXpos(-20 * 4);
	NPC.IncPos(0, -30);
	NPC.IncPos(0, 20 * 2);
	NPC.SetRot(1.5708f);
	//player.SetXpos(-530.0f);
	//player.SetYpos(20.0f);
	player.SetXpos(-20 * 4);
	player.IncPos(0,-30);
	player.IncPos(0,20 * 3);
	player.SetRot(1.5708f);
	std::cout << player.GetTopSpeed();
	bg.loadBackground();
	for (Tile& dirtTile : bg.dirtTiles )
	{
		dirtTile.Init(shader, red);
	}
	for (auto& it : bg.trackTiles)
	{
		it.second.Init(shader, red);
	}
	for (auto& it : bg.checkpoints)
	{
		it.second.Init(shader, red);
	}
	SoundEngine->setSoundVolume(0.5f);
	//SoundEngine->play2D("music/everything.mp3", true);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
			player.IncPos(-((player.GetSpeed() * dt * 10) * sinf(player.GetRot())), ((player.GetSpeed() * dt * 10) * cosf(player.GetRot()))); //same as above
			NPC.SetSpeed(-NPC.GetSpeed() / 2);
			NPC.IncPos(-((NPC.GetSpeed() * dt * 10) * sinf(NPC.GetRot())), ((NPC.GetSpeed() * dt * 10) * cosf(NPC.GetRot()))); //same as above

		}
		switch (PlayersCurrentTile().getID())
		{
		case 'P':
			CompleteLap();
		case 'S':
			if (player.GetYPos() > (PlayersCurrentTile().getYPos() * 20) - 25)
			{
				player.SetSpeed(-player.GetSpeed());
				player.IncPos(-(((player.GetSpeed()) * dt) * sinf(player.GetRot())), (((player.GetSpeed()) * dt) * cosf(player.GetRot())));
			}
			break;
		case 'N':
			CompleteLap();
		case 'W':
			if (player.GetYPos() < (PlayersCurrentTile().getYPos() * 20) - 35)
			{
				player.SetSpeed(-player.GetSpeed());
				player.IncPos(-(((player.GetSpeed()) * dt) * sinf(player.GetRot())), (((player.GetSpeed()) * dt) * cosf(player.GetRot())));
			}
			break;
		case 'D':
			if (-player.GetXPos() > (PlayersCurrentTile().getXPos() * 20) + 5)
			{
				player.SetSpeed(-player.GetSpeed());
				player.IncPos(-(((player.GetSpeed()) * dt) * sinf(player.GetRot())), (((player.GetSpeed()) * dt) * cosf(player.GetRot())));
			}
			break;
		case 'A':
			if (-player.GetXPos() < (PlayersCurrentTile().getXPos() * 20) - 5)
			{
				player.SetSpeed(-player.GetSpeed());
				player.IncPos(-(((player.GetSpeed()) * dt) * sinf(player.GetRot())), (((player.GetSpeed()) * dt) * cosf(player.GetRot())));
			}
			break;
		case 'R':
			//check collision
			xDist = (((PlayersCurrentTile().getXPos() * 20) - 10) - (-player.GetXPos()));
			yDist = ((player.GetYPos()) - ((PlayersCurrentTile().getYPos() * 20) - 20));
			temp = sqrt((xDist * xDist) + (yDist * yDist));
			if (temp > 15.0f)
			{
				player.SetSpeed(-player.GetSpeed());
				player.IncPos(-(((player.GetSpeed()) * dt) * sinf(player.GetRot())), (((player.GetSpeed()) * dt) * cosf(player.GetRot())));
			}
			break;
		case 'V':
			//check collision
			xDist = (((PlayersCurrentTile().getXPos() * 20) - 10) - (-player.GetXPos()));
			yDist = ((player.GetYPos()) - ((PlayersCurrentTile().getYPos() * 20) - 40));
			temp = sqrt((xDist * xDist) + (yDist * yDist));
			if (temp > 15.0f)
			{
				player.SetSpeed(-player.GetSpeed());
				player.IncPos(-(((player.GetSpeed()) * dt) * sinf(player.GetRot())), (((player.GetSpeed()) * dt) * cosf(player.GetRot())));
			}
			break;

		case 'Z':
			//check collision
			xDist = (((PlayersCurrentTile().getXPos() * 20) + 10) - (-player.GetXPos()));
			yDist = ((player.GetYPos()) - ((PlayersCurrentTile().getYPos() * 20) - 40));
			temp = sqrt((xDist * xDist) + (yDist * yDist));
			if (temp > 15.0f)
			{
				player.SetSpeed(-player.GetSpeed());
				player.IncPos(-(((player.GetSpeed()) * dt) * sinf(player.GetRot())), (((player.GetSpeed()) * dt) * cosf(player.GetRot())));
			}
			break;

		case 'Q':
			//check collision
			xDist = (((PlayersCurrentTile().getXPos() * 20) + 10) - (-player.GetXPos()));
			yDist = ((player.GetYPos()) - ((PlayersCurrentTile().getYPos() * 20) - 20));
			temp = sqrt((xDist * xDist) + (yDist * yDist));
			if (temp > 15.0f)
			{
				player.SetSpeed(-player.GetSpeed());
				player.IncPos(-(((player.GetSpeed()) * dt) * sinf(player.GetRot())), (((player.GetSpeed()) * dt) * cosf(player.GetRot())));
			}
			break;
		}
		glm::vec2 tilePos = { std::round(-player.GetXPos() / 20), std::round((player.GetYPos() + 30) / 20) };
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
	}
}

void Game::CompleteLap()
{
	if (checkpointsCompleted == 62)
	{
		for (auto& it : bg.checkpoints)
		{
			it.second.Reset();
		}
		checkpointsCompleted = 0;
		laps++;

		if (laps == 3)
		{
			togglePause();
			gameOver = true;
		}
	}
	else
	{
		std::cout << checkpointsCompleted;
	}
}

Tile& Game::AICurrentTile()
{
	glm::vec2 tilePos = { std::round(-NPC.GetXPos() / 20), std::round((NPC.GetYPos() + 30) / 20) };
	return bg.trackTiles[tilePos];
}

Tile& Game::PlayersCurrentTile()
{
	glm::vec2 tilePos = { std::round(-player.GetXPos() / 20), std::round((player.GetYPos() + 30) / 20) };
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
				NPC.IncRot(-0.05f * dt * NPC.GetSpeed());
			}
			else if (rotInDeg < 180.0f)
			{
				//turn left
				NPC.IncRot(0.05f * dt * NPC.GetSpeed());
			}
			if (NPC.GetSpeed() <= NPC.GetTopSpeed())
			{
				NPC.IncSpeed(0.0001f * dt);
			}
			break;
		case 'S':
			//rotate until facing 0 degrees whilst moving forward;
			if (rotInDeg > 180.0f && rotInDeg < 360.0f)
			{
				//turn left
				NPC.IncRot(0.05f * dt * NPC.GetSpeed());
			}
			if (rotInDeg > 0.0f && rotInDeg <= 180.0f)
			{
				//turn right
				NPC.IncRot(-0.05f * dt * NPC.GetSpeed());

			}
			if (NPC.GetSpeed() <= NPC.GetTopSpeed())
			{
				NPC.IncSpeed(0.0001f * dt);
			}
			break;
		case 'E':
			//rotate until facing 180 whilst moving forward;
			if (rotInDeg > 90.0f)
			{
				//turn right
				NPC.IncRot(-0.05f * dt * NPC.GetSpeed());
			}
			else if (rotInDeg < 90.0f)
			{
				//turn left
				NPC.IncRot(0.05f * dt * NPC.GetSpeed());
			}
			if (NPC.GetSpeed() <= NPC.GetTopSpeed())
			{
				NPC.IncSpeed(0.0001f * dt);
			}
			break;
		case 'W':
			//rotate until facing 180 whilst moving forward;
			if (rotInDeg > 270.0f)
			{
				//turn right
				NPC.IncRot(-0.05f * dt * NPC.GetSpeed());
			}
			else if (rotInDeg < 270.0f)
			{
				//turn left
				NPC.IncRot(0.05f * dt * NPC.GetSpeed());
			}
			if (NPC.GetSpeed() <= NPC.GetTopSpeed())
			{
				NPC.IncSpeed(0.0001f * dt);
			}
			break;
		case 'F':
			if (NPC.GetSpeed() <= NPC.GetTopSpeed())
			{
				NPC.IncSpeed(0.0001f * dt);
			}
			//move forward
			break;
		case 'R':
			//rotate right while moving forward at half speed;
			if (NPC.GetSpeed() <= NPC.GetTopSpeed() / 1.5)
			{
				NPC.IncSpeed(0.0001f * dt);
			}
			else
			{
				NPC.IncSpeed(-0.0001f * dt);
			}
			if (NPC.GetSpeed() > 0.0f)
			{
				NPC.IncRot(-0.05f * dt * NPC.GetSpeed());
			}
			break;
		case'L':
			//rotate left while moving forward at half speed;
			if (NPC.GetSpeed() <= NPC.GetTopSpeed() / 1.5)
			{
				NPC.IncSpeed(0.0001f * dt);
			}
			else
			{
				NPC.IncSpeed(-0.0001f * dt);
			}
			if (NPC.GetSpeed() > 0.0f)
			{
				NPC.IncRot(0.05f * dt * NPC.GetSpeed());
			}
			break;

		default:
			break;

	}
	NPC.IncPos(-((NPC.GetSpeed() * dt) * sinf(NPC.GetRot())), ((NPC.GetSpeed() * dt) * cosf(NPC.GetRot()))); //same as above
}

void Game::ProcessInput(float dt)
{
	if (!gamePaused && !gameOver)
	{
		if (Left && (player.GetSpeed() != 0.0f))
		{
			if (player.GetSpeed() > 0)
			{
				player.IncRot(0.05f * dt * player.GetSpeed());
			}
			else
			{
				player.IncRot(0.05f * dt * player.GetSpeed());
			}
		}
		if (Right && (player.GetSpeed() != 0.0f))
		{
			if (player.GetSpeed() > 0)
			{
				player.IncRot(-0.05f * dt * player.GetSpeed());
			}
			else
			{
				player.IncRot(-(0.05f * dt * player.GetSpeed()));
			}
		}
		if (Up && !Down)
		{
			if (player.GetSpeed() < 0)
			{
				player.IncSpeed(0.0005f * dt);
			}
			if (player.GetSpeed() <= player.GetTopSpeed())
			{
				player.IncSpeed(0.0001f * dt);
			}
		}
		if (Down && !Up)
		{
			if (player.GetSpeed() > 0)
			{
				player.IncSpeed(-0.0005f * dt);
			}
			if (player.GetSpeed() >= -((player.GetTopSpeed() - (player.GetTopSpeed() / 10))))
			{
				player.IncSpeed(-0.0001f * dt);
			}
		}
		if (!Up && !Down)
		{

			if (player.GetSpeed() < 0)
			{
				player.IncSpeed(0.0001f * dt);
			}
			else if (player.GetSpeed() > 0)
			{
				player.IncSpeed(-0.0001 * dt);
			}
			if ((player.GetSpeed() > -0.005 && player.GetSpeed() < 0.0f) || (player.GetSpeed() < 0.005 && player.GetSpeed() > 0.0f))
			{
				player.SetSpeed(0);
			}
		}
		//std::cout << player.GetSpeed();
		player.IncPos(-((player.GetSpeed() * dt) * sinf(player.GetRot())), ((player.GetSpeed() * dt) * cosf(player.GetRot()))); //same as above

	}
}

void Game::Render()
{
		//clear the colour and depth buffers
		glClear(GL_COLOR_BUFFER_BIT);

		//make work
		glm::mat4 cameraMatrix = ProjectionMatrix;
		cameraMatrix = glm::translate(cameraMatrix, glm::vec3(player.GetXPos(), player.GetYPos(), 0.0));
		ViewMatrix = glm::translate(glm::mat4(1.0), glm::vec3(0.0, 0.0, 0.0));

		glm::mat4 ModelViewMatrix = glm::translate(getViewMatrix(), glm::vec3(player.GetXPos(), player.GetYPos(), 0.0));
		glm::mat4 TileViewMatrix;
		ModelViewMatrix = glm::mat4(1.0), glm::vec3(0.0, 0.0, 0.0);
		for (Tile& dirtTile : bg.dirtTiles)
		{
			TileViewMatrix = glm::translate(glm::mat4(1.0), glm::vec3(dirtTile.getXPos() * dirtTile.GetWidth(), (bg.GetMapHeight() - (dirtTile.getYPos() * dirtTile.GetHeight())), 0.0));
			dirtTile.Render(shader, TileViewMatrix, cameraMatrix);
		}
		glEnable(GL_BLEND);

		for (auto& it : bg.trackTiles)
		{
			TileViewMatrix = glm::translate(glm::mat4(1.0), glm::vec3(it.second.getXPos() * it.second.GetWidth(), (bg.GetMapHeight() - (it.second.getYPos() * it.second.GetHeight())), 0.0));
			it.second.Render(shader, TileViewMatrix, cameraMatrix);
		}
		for (auto& it : bg.checkpoints)
		{
			if (it.second.isActive())
			{
				continue;
			}
			TileViewMatrix = glm::translate(glm::mat4(1.0), glm::vec3(it.second.getXPos() * it.second.GetWidth(), (bg.GetMapHeight() - (it.second.getYPos() * it.second.GetHeight())), 0.0));
			it.second.Render(shader, TileViewMatrix, cameraMatrix);
		}

		//player rendering code

		ViewMatrix = glm::translate(glm::mat4(1.0), glm::vec3(0.0, 0.0, 0.0));
		ModelViewMatrix = glm::translate(ViewMatrix, glm::vec3(-player.GetXPos(), -player.GetYPos(), 0.0));
		ModelViewMatrix = glm::rotate(ModelViewMatrix, player.GetRot(), glm::vec3(0.0, 0.0, 1.0));
		player.Render(shader, ModelViewMatrix, cameraMatrix);

		//npc rendering code
		ViewMatrix = glm::translate(glm::mat4(1.0), glm::vec3(0.0, 0.0, 0.0));
		ModelViewMatrix = glm::translate(ViewMatrix, glm::vec3(-NPC.GetXPos(), -NPC.GetYPos(), 0.0));
		ModelViewMatrix = glm::rotate(ModelViewMatrix, NPC.GetRot(), glm::vec3(0.0, 0.0, 1.0));
		NPC.Render(shader, ModelViewMatrix, cameraMatrix);

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



