#include "Game.h"
#include <irrKlang/include/irrKlang.h>



irrklang::ISoundEngine* SoundEngine = irrklang::createIrrKlangDevice();
int count = 0;
Game::Game(unsigned int width, unsigned int height)
{
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
	//player.SetXpos(-530.0f);
	//player.SetYpos(20.0f);
	player.SetXpos(-20 * 3);
	player.IncPos(0,-30);
	player.IncPos(0,20 * 3);
	player.IncRot(3.14/2);
	myOtherSquare.SetWidth(200.0f);
	myOtherSquare.SetHeight(200.0f);
	myOtherSquare.Init(shader, red, "textures/background.png");
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
	//SoundEngine->setSoundVolume(0.5f);
	//SoundEngine->play2D("music/everything.mp3", true);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Game::Update(float dt)
{
	float xDist = 0.0f;
	float yDist = 0.0f;
	float temp = 0.0f;
	glm::vec2 coords;
	switch (PlayersCurrentTile().getID())
	{
	case 'P':
		CompleteLap();
	case 'S':
		if (player.GetYPos() > (PlayersCurrentTile().getYPos() * 20) - 25)
		{
			player.SetSpeed(-player.GetSpeed());
			player.IncPos(-(((player.GetSpeed())* dt) * sinf(player.GetRot())), (((player.GetSpeed())* dt) * cosf(player.GetRot())));
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
	switch(bg.checkpoints[tilePos].getType())
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
}

void Game::CompleteLap()
{
	if (checkpointsCompleted == 62)
	{
		gameOver = true;
		for (auto& it : bg.checkpoints)
		{
			it.second.Reset();
		}
		checkpointsCompleted = 0;
	}
	else
	{
		std::cout << checkpointsCompleted;
	}
}

Tile& Game::PlayersCurrentTile()
{
	glm::vec2 tilePos = { std::round(-player.GetXPos() / 20), std::round((player.GetYPos() + 30) / 20) };
	return bg.trackTiles[tilePos];
}

void Game::ProcessInput(float dt)
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
	player.IncPos(-((player.GetSpeed() *dt)* sinf(player.GetRot())), ((player.GetSpeed()*dt)* cosf(player.GetRot()))); //same as above

}

void Game::Render()
{
	//clear the colour and depth buffers
	glClear(GL_COLOR_BUFFER_BIT);

	//make work
	ViewMatrix = glm::translate(glm::mat4(1.0), glm::vec3(player.GetXPos(), player.GetYPos(), 0.0));

	glm::mat4 ModelViewMatrix = glm::translate(getViewMatrix(), glm::vec3(player.GetXPos(), player.GetYPos(), 0.0));

	//myOtherSquare.Render(shader, ViewMatrix, ProjectionMatrix);

	ModelViewMatrix = glm::mat4(1.0), glm::vec3(0.0, 0.0, 0.0);
	for (Tile& dirtTile : bg.dirtTiles)
	{
		ViewMatrix = glm::translate(glm::mat4(1.0), glm::vec3(dirtTile.getXPos() * dirtTile.GetWidth(), (bg.GetMapHeight() - (dirtTile.getYPos() * dirtTile.GetHeight())), 0.0));
		glm::mat4 ModelViewMatrix = glm::translate(getViewMatrix(), glm::vec3(player.GetXPos(), player.GetYPos(), 0.0));
		dirtTile.Render(shader, ModelViewMatrix, ProjectionMatrix);
	}
	glEnable(GL_BLEND);

	for (auto& it: bg.trackTiles)
	{
		ViewMatrix = glm::translate(glm::mat4(1.0), glm::vec3(it.second.getXPos() * it.second.GetWidth(), (bg.GetMapHeight() - (it.second.getYPos() * it.second.GetHeight())), 0.0));
		glm::mat4 ModelViewMatrix = glm::translate(getViewMatrix(), glm::vec3(player.GetXPos(), player.GetYPos(), 0.0));
		it.second.Render(shader, ModelViewMatrix, ProjectionMatrix);
	}
	for (auto& it : bg.checkpoints)
	{
		if(it.second.isActive())
		{
			continue;
		}
		ViewMatrix = glm::translate(glm::mat4(1.0), glm::vec3(it.second.getXPos() * it.second.GetWidth(), (bg.GetMapHeight() - (it.second.getYPos() * it.second.GetHeight())), 0.0));
		glm::mat4 ModelViewMatrix = glm::translate(getViewMatrix(), glm::vec3(player.GetXPos(), player.GetYPos(), 0.0));
		it.second.Render(shader, ModelViewMatrix, ProjectionMatrix);
	}
	ModelViewMatrix = glm::mat4(1.0), glm::vec3(0.0, 0.0, 0.0);
	ModelViewMatrix = glm::rotate(ModelViewMatrix, player.GetRot(), glm::vec3(0.0, 0.0, 1.0));
	player.Render(shader, ModelViewMatrix, ProjectionMatrix);

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



