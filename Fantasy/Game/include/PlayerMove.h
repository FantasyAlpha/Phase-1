#pragma once

#include <GameComponent.h>
#include <SceneManager.h>
#include <Animator.h>

struct PlayerMove
	: public GameComponent
{
	PlayerMove(char *name, char * colliderName, float goalVelocity,bool Run ,char *type = "PLAYERMOVE") ;
	

	virtual void Init();
	virtual void Destroy(){}
	virtual void Update(float delta, Game_Input *input);
	

	virtual void Render(){}
	float  currentvelocity ;
	char * colliderName;
	float goalVelocity; 
	bool Run;
	Animator *animator;
};