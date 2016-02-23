#pragma once

#include "Game.h"
#include "InputManager.h"
#include "WindowManager.h"

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
static WindowManager Window;
//
static Game_Code Game;
static State state;
static Game_Input Input;
static Input_Keys Keys;
// As long as it is true the main loop will keep running, if it is false the window should exit
static bool IsRunning;


//Instance of the game
//Game *GlobalGame;

// Begin running the engine
void Run();

LRESULT CALLBACK WindowCallBack(HWND window, UINT message, WPARAM wParam, LPARAM lParam);

//Initialize the engine and all systems associated with it (if there is any)
void InitSystem(HINSTANCE hInstance, char *title, int width, int height);

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
static void MainLoop();

//
FILETIME GetLastWriteTime(char *path);
Game_Code LoadGameCode(char *path, char *temp);
void UnloadGameCode(Game_Code *gameCode);


