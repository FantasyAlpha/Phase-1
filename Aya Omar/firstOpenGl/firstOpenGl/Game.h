// this class have game stuff handeling : game state , render, update 
// no os stuff 


#pragma once

#include <GL/glew.h>
#include<SDL2/SDL.h>


// Represents the current state of the game to decide to render and / or to process different data 
enum GameState {
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};

// Game holds all game-related state and functionality.
// Combines all game-related data into a single class for
// easy access to each of the components and manageability.
class Game
{
public:
	// Game state
	GameState              State;

	// array of inputs 
	GLboolean              Keys[1024];

	//GLuint                 Width, Height;
	// Constructor/Destructor
	Game();
	~Game();

	// Initialize game state (load all shaders/textures/levels)
	void Init();

	// GameLoop
	void ProcessInput(/*GLfloat dt*/);
	void Update(/*GLfloat dt*/);
	void Render();
};

