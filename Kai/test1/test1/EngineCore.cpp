#include "EngineCore.h"


EngineCore::EngineCore(char *title, int width, int height)
{
	InitEngine(title, width, height);
}


EngineCore::~EngineCore()
{
	if (Window)
	{
		delete Window;
	}
}

//Initialize the engine and all systems associated with it (if there is any)
void EngineCore::InitEngine(char *title, int width, int height)
{
	Window = new WindowManager();
	Window->InitWindow(title, width, height);
}

//Begin running the engine
void EngineCore::Run()
{
	Running = true;
	MainLoop();
}

//stop the engine
void EngineCore::Stop()
{
	Running = false;
}

//Release resources (if there is any) and destory  the window
void EngineCore::Release()
{
	//Destroy the window
	Window->DestroyWindow();
}

//Our main loop which should continue running as long as we don't quite the game
void EngineCore::MainLoop()
{
	while (Running)
	{
		if (Window->IsCloseRequested())
		{
			Stop();
		}
		//Update the window
		Window->UpdateWindow();
		//Render the window
		Window->RenderWindow();
	}

	//Release resources (if there is any) and destory  the window
	Release();
}