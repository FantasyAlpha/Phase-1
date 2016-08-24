#pragma once


#include "SpriteRenderer.h"
#include "GameComponent.h"
#include "SceneManager.h"
#include "ResourceManager.h"


struct ArcMonster
	: public GameComponent
{
	ArcMonster(char *name, Game_Resources *resources);


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


	int arcMonCount = 0;
	bool arcMon_flag = false;
	bool waitArcMon_falg = false;
	bool startArcMon = false;
	int waitArcMon = 121;
	float arcMonDirection = 35.0;
	float arcMonStrenght = 35.0;
	float x_ArcVelo = 0.0;
	float y_ArcVelo = 0.0;
	float diro = 0;
	bool arcMon_hideFlag = true;
	Game_Resources *Resources;
	

	GameObject *arcMonster;


};