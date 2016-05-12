#pragma once

#include "Game.h"
#include "InputManager.h"
#include "WindowManager.h"
#include <ctime> 
#include <stdint.h>
#include <GUITest.h>
//

float fps;
float frametime;
double delta;

clock_t	end;
clock_t start_loop, end_loop;
LARGE_INTEGER frequency;        // ticks per second
LARGE_INTEGER frequency_loop;        // ticks per second

static LARGE_INTEGER t_previous;
LARGE_INTEGER t_previous_loop;

LARGE_INTEGER t_current;
LARGE_INTEGER t_current_loop;

float max_fps = 60.0f;

struct Game_Code
{
	HMODULE GameCodeDLL;

	game_init *Game_Init;
	game_update *Game_Update;
	game_render *Game_Render;
	game_shutdown *Game_Shutdown;

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
static GUI_Input GUIInput;
static Window_Dimensions Dimensions;
static Input_Keys Keys;
static int64_t TicksPerSecond;
static float UPS;
// As long as it is true the main loop will keep running, if it is false the window should exit
static bool IsRunning;


//Instance of the game
//Game *GlobalGame;

//calculate frame per second 
void calcfps();


// Begin running the engine
void Run();

LRESULT CALLBACK WindowCallBack(HWND window, UINT message, WPARAM wParam, LPARAM lParam);

//Initialize the engine and all systems associated with it (if there is any)
void InitSystem(HINSTANCE hInstance, char *title, int width, int height, int ups);

//stop the engine
void Stop();

//Release resources (if there is any) and destory  the window
void Release();

void ProcessInput(Game_Input *input);

//Our main loop which should continue running as long as we don't quite the game
static void MainLoop();

//
FILETIME GetLastWriteTime(char *path);
Game_Code LoadGameCode(char *path, char *temp);
void UnloadGameCode(Game_Code *gameCode);


