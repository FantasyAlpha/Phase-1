/*=======================================================================================================================
NOTE(kai): This file can:

1) creates a window (using either SDL or GLFW)
2) creates opengl context
=======================================================================================================================*/

#pragma once

#include <GL\glew.h>
#include <GL\wglew.h>

struct WindowManager
{
	HWND Window;
	WNDPROC WindowCallback;
	int Width;
	int Height;	
};

BOOL RegisterWindowClass(HINSTANCE hInstance, char* name, WNDPROC callback);

bool InitGlew(HINSTANCE hInstance);

bool InitOpengl(HINSTANCE hIsntance, WindowManager *window, int versionMajor, int versionMinor);

void InitWindow(HINSTANCE hInstance, WindowManager *window, char* title, int width, int height);

//Renders our window (this should happen each frame)
void RenderWindow(HWND window);

//Print out the opengl version supported by our driver
void PrintOpenglVersion();

//Sets some options for opengl
void SetOpenglOptions();

//Clear the window
void ClearWindow();