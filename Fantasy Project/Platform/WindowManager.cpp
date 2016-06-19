#include "WindowManager.h"
#include <iostream>

BOOL RegisterWindowClass(HINSTANCE hInstance, char* name, WNDPROC callback)
{
	WNDCLASS windowClass = {};
	windowClass.hInstance = hInstance;
	windowClass.lpszClassName = name;
	windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	windowClass.lpfnWndProc = callback;
	windowClass.hCursor = LoadCursor(0, IDC_ARROW);

	return RegisterClass(&windowClass);
}

//NOTE(kai): a context is basically all data within the window ,including attributes, buffers, ...etc
//Initialize a window
void InitWindow(HINSTANCE hInstance, WindowManager *window, char* title, int width, int height, ClearColor color)
{	
	if (RegisterWindowClass(hInstance, title, window->WindowCallback))
	{
		window->Window = CreateWindow(title
									, title
									, WS_OVERLAPPEDWINDOW ^ WS_MAXIMIZEBOX ^ WS_THICKFRAME | WS_VISIBLE
									, CW_USEDEFAULT
									, CW_USEDEFAULT
									, width
									, height
									, NULL
									, NULL
									, hInstance
									, NULL);

		if (window->Window)
		{
			if (InitOpengl(hInstance, window))
			{
				//Print out the opengl version supported by our driver
				PrintOpenglVersion();

				//Sets some options for opengl
				SetOpenglOptions();

				RECT windowRect;
				GetClientRect(window->Window, &windowRect);
				window->Width = windowRect.right;
				window->Height = windowRect.bottom;
				SetViewPort(windowRect.left, windowRect.top, windowRect.right, windowRect.bottom);
				SetClearColor(color);
			}			
			else
			{
				OutputDebugString("Failed to initialize opengl\n");
			}
		}
		else
		{
			OutputDebugString("Failed to create window\n");
		}
	}
	else
	{
		OutputDebugString("Failed to create window class\n");
	}		
}



//Renders our window (this should happen each frame)
void RenderWindow(HWND window)
{
	static char buffer[20];
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		wsprintf(buffer, "OpenGL Error : %d \n", error);
		OutputDebugString(buffer);
	}
	HDC windowDC = GetDC(window);
	SwapBuffers(GetDC(window));
	ReleaseDC(window, windowDC);
}

//Print out the opengl version supported by our driver
void PrintOpenglVersion()
{
	std::cout << glGetString(GL_VENDOR) << ", Opengl version: " << glGetString(GL_VERSION) << "\n";
	OutputDebugString((LPSTR)glGetString(GL_VENDOR));
	OutputDebugString(", Opengl version: ");
	OutputDebugString((LPSTR)glGetString(GL_VERSION));
	OutputDebugString("\n");
}

//Sets some options for opengl
void SetOpenglOptions()
{
	//The color that we will use to clear the screen (painting it with this color)
	glClearColor(0.0f, 0.3f, 0.5f, 1.0f);

	glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

//Clear the window
void ClearWindow()
{
	//Clears the window with the clear color we specified before (we have to specify which buffers we want to clear)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void SetClearColor(ClearColor color)
{
	glClearColor(color.r, color.g, color.b, color.a);
}

void SetViewPort(float x, float y, float width, float height)
{
	//Sets the portion of the window we will render on
	glViewport(x				//The X origin
		, y				//The Y origin
		, width			//Width of the portion we will render on
		, height);			//Height of the portion we will render on
}