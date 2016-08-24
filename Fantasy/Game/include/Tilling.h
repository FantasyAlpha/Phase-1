#pragma once


#include "SpriteRenderer.h"
#include "GameComponent.h"
#include "SceneManager.h"
#include "ResourceManager.h"


struct Tilling
	: public GameComponent
{
	Tilling(char *name, Game_Resources *resources,char* name1, char* name2, vec3f pos, vec2f size, char* textureName, float screenWidth, float zaxis);


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


	bool hasRightBuddy = false;
	bool hasLeftBuddy = false;
	bool reverseScale = false;
	float spriteWidth = 1664.0f;
	int extra = 0;
	char* previousBackGround = "";
	char* backgroundActorName = "";
	bool createNewBackGround = false;
	int rightorleft = 1;
	float previousPosition = 0.0f;
	char* tilingActorName1 = "";
	char* tilingActorName2 = "";
	vec3f myPos;
	vec2f mySize;
	char *textureName;
	char *myName;
	//left and right check for each background
	bool BG0_R = false;
	bool BG0_L = false;
	bool BG1_R = false;
	bool BG1_L = false;
	//position size textureName
	float ScreenWidth = 0.0f;
	Game_Resources *Resources;
	GameObject *tilling1;
	GameObject *tilling2;
	float zAxis = 0.0f;
	float preCameraPos = 0.0f;
	float parallax = 0.0f;
};