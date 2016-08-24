#pragma once 

#include "GameObject.h"

struct GameComponent
{
public:
	virtual ~GameComponent(){}
	
	virtual void PreInit(){}
	virtual void Init(){}
	virtual void Destroy(){}
	virtual void Update(float delta, Game_Input *input){}
	virtual void Render(){}

public:
	SceneManager *Scene;
	GameObject *Owner;
	uint32 Index;
	uint32 Size;

	char *Name;
	char *Type;
};

