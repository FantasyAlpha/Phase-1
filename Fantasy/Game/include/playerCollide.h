#pragma once


#include <SpriteRenderer.h>
#include <GameComponent.h>
#include <SceneManager.h>
#include <ResourceManager.h>


struct playerCollide
	: public GameComponent
{
	playerCollide(char *name, Game_Resources *resources, char *myCollider);


	virtual void PreInit()
	{

	}



	virtual void Init();
	virtual void Destroy(){}
	virtual void Update(float delta, Game_Input *input);

	virtual void Render()
	{
		
	}

	Collider *collide;
	
	char *MyCollider;
	Game_Resources *Resources;

	GameObject *playerHpBar;
	GameObject *mainCanvas;
	float playerHp = 100 ;
	GameObject *me;
	
	


};