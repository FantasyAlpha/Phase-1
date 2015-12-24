/*=======================================================================================================================
NOTE(kai): This file can:
1) creates a window (using either SDL or GLFW)
2) creates opengl context
=======================================================================================================================*/

#pragma once

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <SDL\SDL.h>
#include <iostream>

//Switch between using SDL and GLFW
#define SDL_MODE 0 
#define GLFW_MODE 1
#define MODE_TYPE SDL_MODE
//#define MODE_TYPE GLFW_MODE

struct WindowManager
{
public:
	WindowManager(){}
	~WindowManager(){}

public:
	//NOTE(kai): a context is basically all data within the window ,including attributes, buffers, ...etc
	//Initialize a window
	void InitWindow(char *title, int width, int height);
	//Renders our window (this should happen each frame)
	void RenderWindow();

	//Updates our window (this should happen each frame)
	void UpdateWindow();

	//See if the user wants to close the window
	bool IsCloseRequested();

	//Destroy the library (which results in destroying the window)
	void DestroyWindow();

private:
	//Print out the opengl version supported by our driver
	void PrintOpenglVersion()
	{
		std::cout << glGetString(GL_VENDOR) << ", Opengl version: " << glGetString(GL_VERSION) << "\n";
	}

	//Sets some options for opengl
	void SetOpenglOptions();		

private:
	//Turns to true if the user tries to close the window
	bool ShouldClose = false;

	//Window struct that we will use to create the window
#if MODE_TYPE == GLFW_MODE
	GLFWwindow *Window; //GLFW window
#elif MODE_TYPE == SDL_MODE
	SDL_Window *Window;	//SDL window
	SDL_Event Events;	//Window events
#endif	
};

