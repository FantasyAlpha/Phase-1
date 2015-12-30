#pragma once

#include "Mesh.h"
#include "Shader.h"

class Game
{
public:
	Game();
	~Game();

public:
	//Initialize the game
	void Init();

	//Render the game
	void Render();

	//Update the game
	void Update();

private:
	Mesh *mesh;
	Shader *shader;
};

