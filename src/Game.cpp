#include "Game.h"
#include <irrKlang/include/irrKlang.h>



irrklang::ISoundEngine* SoundEngine = irrklang::createIrrKlangDevice();

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
	player.IncPos(-30,0);
	player.IncRot(3.14/2);
	myOtherSquare.SetWidth(200.0f);
	myOtherSquare.SetHeight(200.0f);
	myOtherSquare.Init(shader, red, "textures/background.png");
	bg.loadBackground("textures/spritesheet_tiles.png", 128.0f, 128.0f);
	for (Tile& dirtTile : bg.dirtTiles )
	{
		dirtTile.Init(shader, red);
	}
	for (Tile& trackTile : bg.trackTiles)
	{
		trackTile.Init(shader, red);
	}
	SoundEngine->setSoundVolume(0.7f);
	SoundEngine->play2D("music/everything.mp3", true);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Game::Update(float dt)
{
	for (int i = 0; i < 900; i++)
	{
		/*if (bg.tiles[i].IsInCollision(player.GetOBB()))
		{
			std::cout << "colliding!";
		}*/
	}
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
		ViewMatrix = glm::translate(glm::mat4(1.0), glm::vec3(((dirtTile.getXPos())*dirtTile.GetWidth() - bg.GetMapWidth()),(bg.GetMapHeight() -(((dirtTile.getYPos())*dirtTile.GetHeight()) - bg.GetMapHeight())),0.0));
		glm::mat4 ModelViewMatrix = glm::translate(getViewMatrix(), glm::vec3(player.GetXPos(), player.GetYPos(), 0.0));
		dirtTile.Render(shader, ModelViewMatrix, ProjectionMatrix);
	}
	glEnable(GL_BLEND);

	for (Tile& trackTile : bg.trackTiles)
	{
		ViewMatrix = glm::translate(glm::mat4(1.0), glm::vec3(((trackTile.getXPos()) * trackTile.GetWidth() - bg.GetMapWidth()), (bg.GetMapHeight() - (((trackTile.getYPos()) * trackTile.GetHeight()) - bg.GetMapHeight())), 0.0));
		glm::mat4 ModelViewMatrix = glm::translate(getViewMatrix(), glm::vec3(player.GetXPos(), player.GetYPos(), 0.0));
		trackTile.Render(shader, ModelViewMatrix, ProjectionMatrix);
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



