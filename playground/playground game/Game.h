#pragma once

#ifdef GAME_DLL_EXPORTS
#define GAME_DLL extern "C" __declspec(dllexport) 
#else
#define GAME_DLL __declspec(dllimport) 
#endif

#include<ctime>
#include <ResourceManager.h>
#include <Components.h>

//

struct Game_Button
{
	int Button;

	bool KeyDown;
	bool KeyUp;
};

#define BUTTON_COUNT 15

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
			Game_Button Space;

			Game_Button W;
			Game_Button S;
			Game_Button A;
			Game_Button D;
			Game_Button Z;

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

void jumpHandle(char*ActorName, Game_Input *input, float delta);

void InitializeComponents();