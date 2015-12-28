#pragma once

#include<iostream>
#include "GL\glew.h"
#include "GLFW\glfw3.h"

class Window {

public :
	Window(int width = 800, int height = 600, char *name = "window" );

	~Window(){}


private:
	GLFWwindow *window;
	int width, height;
	char *name;
	
	void initOpengl();

public:

	void createWindow();
	
	bool shouldClose();
	
	void destroy();
	
	void update();

	void render();
};