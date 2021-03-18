#include <iostream>
using namespace std;

//--- OpenGL ---
#include "..\GL\glew.h"
#include "..\GL\wglew.h"
#pragma comment(lib, "glew32.lib")
//--------------

#include "..\glm\glm.hpp"
#include "..\glm\gtc\matrix_transform.hpp"

#include "..\shaders\Shader.h"
#include "..\GL\freeglut.h"
#include "..\FreeImage.h"

#include "Player.h"

#include <iostream>
using namespace std;

glm::mat4 ViewMatrix;  // matrix for the modelling and viewing
glm::mat4 ProjectionMatrix; // matrix for the orthographic projection
int screenWidth = 1280, screenHeight = 720;

//booleans to handle when the arrow keys are pressed or released.
bool Left = false;
bool Right = false;
bool Up = false;
bool Down = false;
GLint tempX = 0;
GLint tempY = 0;
Shader shader;
Player player(0.25);
Sprite myOtherSquare;

//OPENGL FUNCTION PROTOTYPES
void display();				//used as callback in glut for display.
void reshape(int width, int height); //used as callback for reshape function in glut
void init();				//called in main when the program starts.

/*************    START OF OPENGL FUNCTIONS   ****************/

void reshape(int width, int height)		// Resize the OpenGL window
{
	screenWidth = width;
	screenHeight = height;

	glViewport(0,0, width, height);						// set Viewport dimensions

	ProjectionMatrix = glm::ortho(width/-25.0, width/25.0, height/-25.0, height/25.0); 
}


void display()									
{
	//clear the colour and depth buffers
	glClear(GL_COLOR_BUFFER_BIT);

	//make work
	ViewMatrix = glm::translate(glm::mat4(1.0), glm::vec3(player.GetXPos(), player.GetYPos(), 0.0));

	glm::mat4 ModelViewMatrix = glm::translate(ViewMatrix, glm::vec3(player.GetXPos(), player.GetYPos(), 0.0));

	myOtherSquare.Render(shader, ViewMatrix, ProjectionMatrix);

	glEnable(GL_BLEND);
ModelViewMatrix = glm::mat4(1.0), glm::vec3(0.0, 0.0, 0.0);
ModelViewMatrix = glm::rotate(ModelViewMatrix, player.GetRot(), glm::vec3(0.0, 0.0, 1.0));
player.Render(shader, ModelViewMatrix, ProjectionMatrix);

glDisable(GL_BLEND);


glutSwapBuffers();

}

void init()
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

void special(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		Left = true;
		break;
	case GLUT_KEY_RIGHT:
		Right = true;
		break;
	case GLUT_KEY_UP:
		Up = true;
		break;
	case GLUT_KEY_DOWN:
		Down = true;
		break;
	}
}

void specialUp(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		Left = false;
		break;
	case GLUT_KEY_RIGHT:
		Right = false;
		break;
	case GLUT_KEY_UP:
		Up = false;
		break;
	case GLUT_KEY_DOWN:
		Down = false;
		break;
	}
}

void processKeys()
{
	if (Left && (player.GetSpeed() != 0.0f))
	{
		player.IncRot(0.05f * player.GetSpeed());
	}
	if (Right && (player.GetSpeed() != 0.0f))
	{
		player.IncRot(-0.05f * player.GetSpeed());
	}
	if (Up)
	{
		if (player.GetSpeed() < 0)
		{
			player.IncSpeed(0.005f);
		}
		if (player.GetSpeed() <= player.GetTopSpeed())
		{
			player.IncSpeed(0.001f);
		}
	}
	if (Down)
	{
		if (player.GetSpeed() > 0)
		{
			player.IncSpeed(-0.005f);
		}
		if (player.GetSpeed() >= -(player.GetTopSpeed() - (player.GetTopSpeed()/10)))
		{
			player.IncSpeed(-0.001f);
		}
	}
	if (!Up && !Down)
	{
		
		if (player.GetSpeed() < 0)
		{
			player.IncSpeed(0.001f);
		}
		else if (player.GetSpeed() > 0)
		{
			player.IncSpeed(-0.001);
		}
		if ((player.GetSpeed() > -0.005 && player.GetSpeed() < 0.0f) || (player.GetSpeed() < 0.005 && player.GetSpeed() > 0.0f))
		{
			player.SetSpeed(0);
		}
	}
	player.IncPos(-(player.GetSpeed() * sinf(player.GetRot())), (player.GetSpeed() * cosf(player.GetRot()))); //same as above

}

void idle()
{
	processKeys();

	glutPostRedisplay();
}
/**************** END OPENGL FUNCTIONS *************************/

// FREEGLUT WINDOW SET UP
int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(screenWidth, screenHeight);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("OpenGL FreeGLUT Example: Image loading");

	glutReshapeFunc(reshape);

	//This initialises glew - it must be called after the window is created.
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		cout << " GLEW ERROR" << endl;
	}

	//Check the OpenGL version being used
	int OpenGLVersion[2];
	glGetIntegerv(GL_MAJOR_VERSION, &OpenGLVersion[0]);
	glGetIntegerv(GL_MINOR_VERSION, &OpenGLVersion[1]);
	cout << OpenGLVersion[0] << " " << OpenGLVersion[1] << endl;

	//initialise the objects for rendering
	init();

	//specify which function will be called to refresh the screen.
	glutDisplayFunc(display);

	glutSpecialFunc(special);
	glutSpecialUpFunc(specialUp);

	glutIdleFunc(idle);

	//starts the main loop. Program loops and calls callback functions as appropriate.
	glutMainLoop();

	return 0;
}

