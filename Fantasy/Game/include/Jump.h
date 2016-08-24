#pragma once

#include <GameComponent.h>
#include <SceneManager.h>
#include <SpriteRenderer.h>
#include <ResourceManager.h>
#include <Animator.h>
#include <playerCollide.h>

struct Jump
	: public GameComponent
{
	Jump(char *name, float speed, Game_Resources *resources, char *type = "JUMP");

	
	virtual void Init();
	virtual void Destroy(){}
	virtual void Update(float delta, Game_Input *input);

	virtual void Render(){}

	float Speed;
	float fallSpeed_jump ;
	float fallSpeed ;
	bool jumpflag;

	bool doubleJumpFlag ;
	bool onair ;
	int doublejump ;
	bool waitBeforeDouble_flag ;
	int waitDouble ;
	int  controlWait;
	GameObject *jumboo;
	Game_Resources *Resources;
	Animator *animator;
	bool jumbooHide_flag = true;
};