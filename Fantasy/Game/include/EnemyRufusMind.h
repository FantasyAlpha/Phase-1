#pragma once


#include <SpriteRenderer.h>
#include <GameComponent.h>
#include <SceneManager.h>
#include <ResourceManager.h>

class EnemyRufusMind : public GameComponent
{
public:
	EnemyRufusMind(){}
	EnemyRufusMind(char* comName, Game_Resources *resources, char* name, vec3f pos, char* attackName, char* attackName2,
		char* hbBarName, char* idleAName, char* walkAname, char* attackAName, char* attack2AName, char* colliderName, char* spriteName);
	~EnemyRufusMind(){}
public:
	virtual void Update(float delta, Game_Input *input);
	virtual void Init();
public:
	
public:
	char* myName;
	char* myAttackName;
	char* myAttackAdvanceName;
	char* myHbBarName;
	char* myIdleAName;
	char* myWalkAName;
	char* myAttackAName;
	char* myAttackAdvanceAname;
	float seeRange = 700;
	float attackRange = 150;
	float myHp = 100;

	int waitAttackTime = 220;
	bool isAttackWaiting = false;
	int waitAttackTimer = 0;
	bool showColiderAfter = false;
	int showColiderAfterTimer = 0;
	bool fireAttackCollisionL = false;
	bool showColiderAfterR = false;
	bool fireAttackCollisionR = false;

	bool showColiderAfterAdvance = false;
	int showColiderAfterAdvanceTimer = 0;
	bool fireAttackCollisionAdvanceL = false;
	bool showColiderAfterAdvanceR = false;
	bool fireAttackCollisionAdvanceR = false;



	bool attackCollisionOn = true;
	bool attackCollisionOnAdvance = true;


	bool holdColide = false;
	int holdColideTimer = 0;

	char* myColliderName;
	char* mySpriteName;

	Collider *k;
	SpriteRenderer *s;
	vec3f myPos;

	Game_Resources *Resources;

	GameObject *HpBar;
	GameObject *Attack;
	GameObject *AttackAdvance;
	GameObject *player;
	GameObject *me;
};
