#include "Game.h"


Game::Game()
{
}


Game::~Game()
{
	if (m_Mesh)
	{
		delete m_Mesh;
	}

	if (m_Shader)
	{
		delete m_Shader;
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

	m_Shader = new Shader("shaders\\vertex shader 120.vert", "shaders\\fragment shader 120.frag");
	m_Mesh = new Mesh(vertices, sizeof(vertices) / sizeof(Vertex), indices, sizeof(indices) / sizeof(unsigned int));
	
}

//Render the game
void Game::Render()
{
	m_Shader->Activate();
	m_Mesh->Draw();
}

//Update the game
void Game::Update()
{

}