#pragma once


#include "SpriteRenderer.h"
#include "GameComponent.h"
#include "SceneManager.h"
#include "ResourceManager.h"


struct StraightMonster
	: public GameComponent
{
	StraightMonster(char *name, Game_Resources *resources);


	virtual void PreInit()
	{

	}



	virtual void Init();
	virtual void Destroy(){}
	virtual void Update(float delta, Game_Input *input);

	virtual void Render()
	{
		//DrawSprite(&SpriteData);
	}


	int straightCount = 0;
	bool straight_flag = false;
	bool waitStraight_falg = false;
	int waitStraight = 121;
	bool straight_hideFlag = true;
	bool startStraightMon = false;
	Game_Resources *Resources;
	GameObject *straightMonster;


};