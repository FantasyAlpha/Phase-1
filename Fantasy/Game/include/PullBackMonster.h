#pragma once


#include <SpriteRenderer.h>
#include <GameComponent.h>
#include <SceneManager.h>
#include <ResourceManager.h>


struct PullBackMonster
	: public GameComponent
{
	PullBackMonster(char *name, Game_Resources *resources);


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


	int pullCount = 0;
	bool pullBack_flag = false;
	bool waitPull_falg = false;
	bool startPullBackMon = false;
	bool pullBackHide_flag = true;
	int waitPull = 121;

	Game_Resources *Resources;

	GameObject *pullBackMonster;


};