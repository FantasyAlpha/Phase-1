#pragma once

#include "Game.h"

class EnemyWarrior
{
public:
	EnemyWarrior(){}
	EnemyWarrior(SceneManager *scene, Game_Resources *resources, char* name, vec3f pos, vec2f size, char* textureName, char* attackName, char* hbBarName,char* idleAName,char* walkAname,char* attackAName);
	~EnemyWarrior(){}
public:
	void Update(void);
	char* GetName(void);
public:
	SceneManager *Scene;
	Game_Resources *Resources;
public:
	char* myName;
	char* myAttackName;
	char* myHbBarName;
	char* myIdleAName;
	char* myWalkAName;
	char* myAttackAName;
	float seeRange = 500;
	float attackRange = 200;
	float myHp = 100;

	int waitAttackTime = 220;
	bool isAttackWaiting = false;
	int waitAttackTimer = 0;
	bool showColiderAfter = false;
	int showColiderAfterTimer = 0;
	bool fireAttackCollisionL = false;
	bool showColiderAfterR = false;
	bool fireAttackCollisionR = false;

	bool attackCollisionOn = false;


	bool holdColide = false;
	int holdColideTimer = 0;
};
