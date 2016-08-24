#pragma once

#include <GameComponent.h>
#include <SceneManager.h>

struct ApplyGravity
	: public GameComponent
{
	ApplyGravity(char *name, char * colliderName, char *type = "APPLYGRAVITY");
	virtual void Init();
	virtual void Destroy(){}
	virtual void Update(float delta, Game_Input *input);

	virtual void Render(){}
	float fallSpeed;
	char * colliderName;
};