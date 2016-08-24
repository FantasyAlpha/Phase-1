#pragma once

#include <GL\glew.h>
#include <GL\wglew.h>

struct ClearColor
{
	float r, g, b, a;
};

struct WindowManager
{
	HWND Window;
	WNDPROC WindowCallback;
	ClearColor Color;
	unsigned int WindowFlags;
	int Width;
	int Height;	
};

BOOL RegisterWindowClass(HINSTANCE hInstance, char* name, WNDPROC callback);

bool InitGlew(HINSTANCE hInstance);

bool InitOpengl(HINSTANCE hIsntance, WindowManager *window);

void InitWindow(HINSTANCE hInstance, WindowManager *window, char* title, int width, int height, ClearColor color = ClearColor{ 0.0f, 0.3f, 0.4f, 1.0f });

//Renders our window (this should happen each frame)
void RenderWindow(HWND window);

//Print out the opengl version supported by our driver
void PrintOpenglVersion();

//Sets some options for opengl
void SetOpenglOptions();

//Clear the window
void ClearWindow();

void SetClearColor(ClearColor color);
void SetViewPort(float x, float y, float width, float height);