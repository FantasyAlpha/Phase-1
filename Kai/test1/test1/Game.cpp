#include "Game.h"


Game::Game()
{
}


Game::~Game()
{
	if (mesh)
	{
		delete mesh;
	}

	if (shader)
	{
		delete shader;
	}
}

//Initialize the game
void Game::Init()
{
	Vertex vertices[] = 
	{ 
		Vertex(Position(0.5f, 0.5f, 0), Color(0, 1, 0, 1)),
		Vertex(Position(0.5f, -0.5f, 0), Color(0, 1, 0, 1)),
		Vertex(Position(-0.5f, -0.5f, 0), Color(1, 0, 0, 1)),
		Vertex(Position(-0.5f, 0.5f, 0), Color(0, 0, 1, 1)),
	};

	unsigned int indices[] = 
	{
		0,
		1, 
		3,
		1,
		2, 
		3,
	};

	shader = new Shader("shaders\\vertex shader 120.vert", "shaders\\fragment shader 120.frag");
	mesh = new Mesh(vertices, sizeof(vertices) / sizeof(Vertex), indices, sizeof(indices) / sizeof(unsigned int));
	
}

//Render the game
void Game::Render()
{
	shader->Activate();
	mesh->Draw();
}

//Update the game
void Game::Update()
{

}