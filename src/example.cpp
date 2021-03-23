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
#include "Game.h"
#include "Player.h"

#include <iostream>
using namespace std;

glm::mat4 ViewMatrix;  // matrix for the modelling and viewing
glm::mat4 ProjectionMatrix; // matrix for the orthographic projection
int screenWidth = 2560, screenHeight = 1440;
float deltaTime = 0.0f;
float lastFrame = 0.0f;


GLint tempX = 0;
GLint tempY = 0;
Shader shader;
Player player(0.25);
Sprite myOtherSquare;
Game racingGame(screenWidth, screenHeight);

//OPENGL FUNCTION PROTOTYPES
void display();				//used as callback in glut for display.
void reshape(int width, int height); //used as callback for reshape function in glut
/*************    START OF OPENGL FUNCTIONS   ****************/

void reshape(int width, int height)		// Resize the OpenGL window
{
	screenWidth = width;
	screenHeight = height;

	glViewport(0,0, width, height);						// set Viewport dimensions

	racingGame.setProjMatrix((glm::ortho(width / -25.0, width / 25.0, height / -25.0, height / 25.0)));
}


void display()									
{
	racingGame.Render();
}

void special(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		racingGame.setLeft(true);
		break;
	case GLUT_KEY_RIGHT:
		racingGame.setRight(true);
		break;
	case GLUT_KEY_UP:
		racingGame.setUp(true);
		break;
	case GLUT_KEY_DOWN:
		racingGame.setDown(true);
		break;
	}
}

void specialUp(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		racingGame.setLeft(false);
		break;
	case GLUT_KEY_RIGHT:
		racingGame.setRight(false);
		break;
	case GLUT_KEY_UP:
		racingGame.setUp(false);
		break;
	case GLUT_KEY_DOWN:
		racingGame.setDown(false);
		break;
	}
}

void idle()
{
	float currentFrame = glutGet(GLUT_ELAPSED_TIME);
	deltaTime = (currentFrame - lastFrame);
	lastFrame = currentFrame;
	racingGame.ProcessInput(deltaTime);
	racingGame.Update(deltaTime);
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
	racingGame.Init();

	//init();

	//specify which function will be called to refresh the screen.
	glutDisplayFunc(display);

	glutIdleFunc(idle);

	glutSpecialFunc(special);
	glutSpecialUpFunc(specialUp);
	glutFullScreenToggle();
	//starts the main loop. Program loops and calls callback functions as appropriate.
	glutMainLoop();

	return 0;
}

