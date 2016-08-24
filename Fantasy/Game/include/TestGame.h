#pragma once

#include <BasicGame.h>
#include <SpriteRenderer.h>
#include <Move.h>
#include <Jump.h>
#include <ApplyGravity.h>
#include <PlayerMove.h>
#include <Collider.h>
#include <ArcMonster.h>
#include <Tilling.h>
#include <BasicAttack.h>
#include <StraightMonster.h>
#include <PullBackMonster.h>
#include <Animator.h>
#include <EnemyWarriorMind.h>
#include <EnemyWizardMind.h>
#include <EnemyRufusMind.h>
#include <playerCollide.h>

struct TestGame
	: public BasicGame
{
public:
	TestGame(){}
	~TestGame(){}
public:
	virtual void Init();	
};