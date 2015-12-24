/*=======================================================================================================================
NOTE(kai): This file can:
1) Intialize the window
2) Initialize any engine subsystem (if there is any)
3) run the engine
=======================================================================================================================*/

#pragma once

#include "WindowManager.h"

struct EngineCore
{
public:
	EngineCore(char *title = "Test Engine", int width = 640, int height = 480);
	~EngineCore();

public:
	//Begin running the engine
	void Run();

private:
	//Initialize the engine and all systems associated with it (if there is any)
	void InitEngine(char *title, int width, int height);

	//stop the engine
	void Stop();

	//Release resources (if there is any) and destory  the window
	void Release();

	//Our main loop which should continue running as long as we don't quite the game
	void MainLoop();

private:
	// As long as it is true the main loop will keep running, if it is false the window should exit
	bool Running = false;

	//Instance of the window manager
	WindowManager *Window;
};

