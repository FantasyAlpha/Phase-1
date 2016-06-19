#pragma once
#include "game.h"

class Player
{
public:
	Player(){}
	Player(SceneManager* scene, Game_Resources* resource, char* name, vec3f pos, vec2f size, char* textureName);
	void Move(char *ActorName, vec2f goalVelocity, float delta, Game_Input *input, bool Run);
	void jumpHandle(char*ActorName, Game_Input *input, float delta);
	void StraigthMonster(char *ActorName, float delta, Game_Input *input);
	void ArcMonster(char *ActorName, float delta, Game_Input *input);
	void PullBack(char *ActorName, vec2f goalVelocity, float delta, Game_Input *input);
	void Colide();
	void BasicAttack(Game_Input *input);
	void Update(Game_Input* input);

	~Player(){}

public:
	SceneManager* sceneManager;
	Game_Resources* gameResource;
	char* myName;
	vec2f currentvelocity = vec2f(0, 0);

	float myHp = 100;



	float fallSpeed_jump = 0.0f;
	float fallSpeed = 0.0f;
	bool jumpflag = false;
	//float Gravity = 2.0f;
	float jumpSpeed = 17.0f;
	bool doubleJumpFlag = false;
	bool onair = false;
	int doublejump = 0;
	bool waitBeforeDouble_flag = false;
	int waitDouble = 121;
	int  controlWait = 0;

	int straightCount = 0;
	bool straight_flag = false;
	bool waitStraight_falg = false;
	int waitStraight = 121;
	bool straight_hideFlag = true;

	int arcMonCount = 0;
	bool arcMon_flag = false;
	bool waitArcMon_falg = false;
	int waitArcMon = 121;
	float arcMonDirection = 25.0;
	float arcMonStrenght = 30.0;
	float x_ArcVelo = 0.0;
	float y_ArcVelo = 0.0;
	float diro = 0;
	bool arcMon_hideFlag = true;


	int pullCount = 0;
	bool pullBack_flag = false;
	bool waitPull_falg = false;
	int waitPull = 121;



	int waitAttackTime = 120;
	bool isAttackWaiting = false;
	int waitAttackTimer = 0;
	bool showColiderAfter = false;
	int showColiderAfterTimer = 0;
	bool fireAttackCollisionL = false;
	bool showColiderAfterR = false;
	bool fireAttackCollisionR = false;

	bool attackCollisionOn = false;
};