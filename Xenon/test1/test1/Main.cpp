
#include<iostream>

#include "GL\glew.h"
#include "GLFW\glfw3.h"


//Global variable
GLFWwindow *window;


void initOpengl()
{

	//initialize GLEW to use openGl functions
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		system("PAUSE");
	}

	glViewport(0, 0, 800, 600);

}


void inputHandle(GLFWwindow *window, int key, int scan, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

void createWindow(int weidth, int height, char *name)
{
	
	glfwInit();
	//window content attributes
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);  //min version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);  //max version
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);  // shader code "for now"
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	//creating window 
	window = glfwCreateWindow(800, 600, "TEST", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Faild to create window \n";
		glfwTerminate();
		system("PAUSE");
	}

	//attach attrib to this window
	glfwMakeContextCurrent(window);

	initOpengl();

	//setting input handler
	glfwSetKeyCallback(window, inputHandle);
}


bool shouldClose()
{
	return glfwWindowShouldClose(window);
}

void destroy()
{
	glfwTerminate();
}


// update and render shapes
void update()
{
	glfwPollEvents();   //input event
	glfwSwapBuffers(window);  // rendering 

}


//main loop
void run()
{
	//main loop 
	while (!shouldClose())
	{
		update();
	}

	destroy();
}



int main()
{

	createWindow(800, 600, "test");

	run();
	
	system("PAUSE");
	
	return 0;
}