#include "Game.h"

shader *m_shader;

SpriteRenderer *renderer;

Texture *texture;

// intialize game variables 
Game::Game(GLuint width, GLuint height)
: State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{
	
}

Game::~Game()
{
	delete renderer;
}

void Game::Init()
{
 m_shader=new shader ("res\\basicShader\\basicshader,vs.txt", "res\\basicShader\\basicshader,fs.txt");
 
  texture= new Texture ("res\\basicShader\\bricks .jpg");
  //renderer = new SpriteRenderer(/*ResourceManager::GetShader("sprite")*/);
  renderer->initRenderMesh();
  m_shader->updateTransform(renderer->gettransform(glm::vec2(200, 200), glm::vec2(300, 400)));

}

void Game::Update(/*GLfloat dt*/)
{

}


void Game::ProcessInput(/*GLfloat dt*/)
{

}

void Game::Render()
{
	m_shader->bind();
	renderer->DrawSprite( /*texture ,*/ glm::vec2(200, 200), glm::vec2(300, 400), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
}