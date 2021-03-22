#include "Game.h"


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

	glClearColor(0.0, 0.0, 1.0, 0.0);						//sets the clear colour to black

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

	myOtherSquare.SetWidth(200.0f);
	myOtherSquare.SetHeight(200.0f);
	myOtherSquare.Init(shader, red, "textures/background.png");

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Game::Update(float dt)
{

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
	std::cout << player.GetSpeed();
	player.IncPos(-((player.GetSpeed() *dt)* sinf(player.GetRot())), ((player.GetSpeed()*dt)* cosf(player.GetRot()))); //same as above

}

void Game::Render()
{
	//clear the colour and depth buffers
	glClear(GL_COLOR_BUFFER_BIT);

	//make work
	ViewMatrix = glm::translate(glm::mat4(1.0), glm::vec3(player.GetXPos(), player.GetYPos(), 0.0));

	glm::mat4 ModelViewMatrix = glm::translate(getViewMatrix(), glm::vec3(player.GetXPos(), player.GetYPos(), 0.0));

	myOtherSquare.Render(shader, ViewMatrix, ProjectionMatrix);

	glEnable(GL_BLEND);
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



