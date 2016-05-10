/*=======================================================================================================================
NOTE(kai): This file can:

1) Create a mesh
2) Create a shader
3) Draw the mesh

NOTE(kai): This is a test game
=======================================================================================================================*/

#pragma once

#ifdef GAME_DLL_EXPORTS
#define GAME_DLL extern "C" __declspec(dllexport) 
#else
#define GAME_DLL __declspec(dllimport) 
#endif
#include<ctime>
#include <Mesh.h>
#include <Shader.h>
#include <Transform.h>
#include <ResourceManager.h>
#include <MemoryAllocator.h>
#include <Components.h>
//

struct Game_Button
{
	int Button;

	bool KeyDown;
	bool KeyUp;
};

#define BUTTON_COUNT 13

struct Game_Input
{
	union 
	{
		Game_Button Buttons[BUTTON_COUNT];
	
		struct 
		{
			Game_Button UP;
			Game_Button DOWN;
			Game_Button RIGHT;
			Game_Button LEFT;

			Game_Button W;
			Game_Button S;
			Game_Button A;
			Game_Button D;

			Game_Button P;
			Game_Button O;

			Game_Button MOUSE_LEFT;
			Game_Button MOUSE_RIGHT;
			Game_Button MOUSE_MIDDLE;
		};
	};

	vec2f MousePos = vec2f();
};

struct Window_Dimensions
{
	float Width;
	float Height;
};

//Initialize the game
#define GAME_INIT(name) void name(Window_Dimensions dimensions)
typedef GAME_INIT(game_init);

//Update the game
#define GAME_UPDATE(name) void name(Game_Input *input, float delta)
typedef GAME_UPDATE(game_update);

//Render the game
#define GAME_RENDER(name) void name(void)
typedef GAME_RENDER(game_render);

#define GAME_SHUTDOWN(name) void name(void)
typedef GAME_SHUTDOWN(game_shutdown);

//compute time 
void computetime(clock_t start, clock_t end);

void InitEverything();
void InitResources();
void InitGameWorld();

// Movement 


struct AutomaticDirections
{
	bool Rigth = true;
	bool Left = false;
	int counter = 0;
};

void AutomaticMove(char *ActorName, float velocity, float delta);

float Accelerate(float flGoal, float flCurrent, float delta);

void Move(char *ActorName, vec2f goalVelocity, float delta, Game_Input *input, bool Run);


void InitializeComponents();