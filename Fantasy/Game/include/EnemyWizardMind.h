#pragma once


#include <SpriteRenderer.h>
#include <GameComponent.h>
#include <SceneManager.h>
#include <ResourceManager.h>


class EnemyWizardMind
	: public GameComponent
{
public:
	EnemyWizardMind(){}
	EnemyWizardMind(char* comName, Game_Resources *resources, char* name, vec3f pos, char* attackName, char* hbBarName, char* idleAName, char* walkAname, char* attackAName, char* colliderName, char* spriteName);
	~EnemyWizardMind(){}
public:
	virtual void Update(float delta, Game_Input *input);
	virtual void Init();
public:

public:
	char* myName;
	char* myAttackName;
	char* myHbBarName;
	char* myIdleAName;
	char* myWalkAName;
	char* myAttackAName;
	float seeRange = 1500;
	float attackRange = 1000;
	float myHp = 100;

	int waitAttackTime = 220;
	bool isAttackWaiting = false;
	int waitAttackTimer = 0;
	bool showColiderAfter = false;
	int showColiderAfterTimer = 0;
	bool fireAttackCollisionL = false;
	bool showColiderAfterR = false;
	bool fireAttackCollisionR = false;

	bool movingProjectile = false;
	int movingProjectileTimer = 0;
	bool movingProjectileEnd = false;
	float playerposx;
	float playerposy;
	float stepX;
	float stepY;

	bool attackCollisionOn = false;


	bool holdColide = false;
	int holdColideTimer = 0;


	char* myColliderName;
	char* mySpriteName;

	Collider *k;
	SpriteRenderer *s;
	Collider *ownerCollider;
	vec3f myPos;

	Game_Resources *Resources;

	GameObject *HpBar;
	GameObject *Attack;
	GameObject *player;
	GameObject *me;
};
