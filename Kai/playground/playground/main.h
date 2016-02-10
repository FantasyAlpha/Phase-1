#pragma once

#include "Game.h"
#include "windows.h"
#include "Platform Layer\WindowManager.h"
#include "Platform Layer\InputManager.h"

//
struct Game_Code
{
	HMODULE GameCodeDLL;

	game_init *Game_Init;
	game_update *Game_Update;
	game_render *Game_Render;

	FILETIME LastWriteTimeDLL;
	FILETIME LastWriteTimePDB;
};

struct State
{
	char DLLFilePath[MAX_PATH];
	char *OnePastLastSlash;
};

//Instance of the window manager
WindowManager Window;
//
Game_Code Game;
State state;
Game_Input Input;


//Instance of the game
//Game *GlobalGame;

// Begin running the engine
void Run();

//Initialize the engine and all systems associated with it (if there is any)
void InitSystem(char *title, int width, int height);

//stop the engine
void Stop();

//Release resources (if there is any) and destory  the window
void Release();

//Render all of the subsystems in the engine
void Render();
//Update all of the subsystems in the engine
void Update();

void ProcessInput(Game_Input *input);

//Our main loop which should continue running as long as we don't quite the game
void MainLoop();

//
FILETIME GetLastWriteTime(char *path);
Game_Code LoadGameCode(char *path, char *temp);
void UnloadGameCode(Game_Code *gameCode);

// As long as it is true the main loop will keep running, if it is false the window should exit
bool Running = false;

