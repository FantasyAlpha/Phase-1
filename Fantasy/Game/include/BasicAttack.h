#pragma once


#include "SpriteRenderer.h"
#include "GameComponent.h"
#include "SceneManager.h"
#include "ResourceManager.h"


struct BasicAttack
	: public GameComponent
{
	BasicAttack(char *name, Game_Resources *resources);


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

	int waitAttackTime = 120;
	bool isAttackWaiting = false;
	int waitAttackTimer = 0;
	bool showColiderAfter = false;
	int showColiderAfterTimer = 0;
	bool fireAttackCollisionL = false;
	bool showColiderAfterR = false;
	bool fireAttackCollisionR = false;

	bool attackCollisionOn = false;

	Game_Resources *Resources;

	GameObject *basicAttack;


};