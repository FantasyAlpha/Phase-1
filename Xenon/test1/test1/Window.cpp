#include "Window.h"

Window::Window(int width, int height, char *name)
{
	this->width = width;
	this->height = height;
	this->name = name;
}

void Window::initOpengl()
{
	//initialize GLEW to use openGl functions
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		system("PAUSE");
	}

	glViewport(0, 0, width, height);
}

void Window::createWindow()
{

	glfwInit();
	//window content attributes
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);  //min version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);  //max version
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);  // shader code "for now"
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	//creating window 
	window = glfwCreateWindow(width, height, name, NULL, NULL);
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
	//TODO:	glfwSetKeyCallback(window, inputHandle);
}

bool Window::shouldClose()
{
	return glfwWindowShouldClose(window);
}

void Window::destroy()
{
	glfwTerminate();
}

// update and render shapes
void Window::update()
{
	glfwPollEvents();   //input event
}

void Window::render()
{
	glfwSwapBuffers(window);
}

