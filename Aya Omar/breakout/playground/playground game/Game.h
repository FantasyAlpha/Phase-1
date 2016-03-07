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

#include <Mesh.h>
#include<Shader.h>
#include <Transform.h>
#include"GameObject.h"
#include"gameLevel.h"
#include<windows.h>
#include<stdio.h>
#include <math.h>
#include<tuple>
global_variable float screenWidth=800 ;
global_variable float screenHeight=600;

// game player constants 
const vec2 playerSize(100, 20);
//float playervelocity =.001 ;
const vec2  playerVelocity(10);

// intilize bALL game object with constant size and velocity in x

 vec2 ballvelocity(5,6);
const GLfloat ballraduis = 12.5;

//----------------------------------------------------------------------------


	// Represents the four possible (collision) directions
	enum Direction {
		UP,
		RIGHT,
		DOWN,
		LEFT
	};
	// Defines a Collision typedef that represents collision data
	typedef std::tuple<GLboolean, Direction, vec3> Collision;



Direction VectorDirection(vec3 target);

//--------------------------------------------------------------------------------


struct Game_Button
{
	int Button;

	bool KeyDown;
	bool KeyUp;
};

struct Game_Input
{
	Game_Button UP;
	Game_Button DOWN;
	Game_Button RIGHT;
	Game_Button LEFT;	
};

Game_Input in1;

//Initialize the game
#define GAME_INIT(name) void name(void)
typedef GAME_INIT(game_init);

//Update the game
#define GAME_UPDATE(name) void name(Game_Input *input)
typedef GAME_UPDATE(game_update);

//Render the game
#define GAME_RENDER(name) void name(void)
typedef GAME_RENDER(game_render);

//collisions
Collision CheckBoxesCollision(GameObject &one, GameObject &two);  // AABB - AABB collision
vec3 clamp(vec3 value, vec3 minimum, vec3 maximum);
Collision checkBallBoxCollisions(GameObject &ball, GameObject & box);

void DoCollisions();

