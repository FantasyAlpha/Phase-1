#pragma once

#include "GameComponent.h"

struct Collider	: public GameComponent
{
public:
	Collider(char *name, vec3f *pos, vec2f size, vec2f offset, bool ground, bool trigger)
	{
		this->Type = "COLLIDER";
		this->Name = name;
		this->pos = pos;
		this->size = size;
		this->ground = ground;
		this->trigger = trigger;
		this->offset = offset;

		this->wall = false;

		this->detected = 0;
		this->normal = vec2f();
		this->penetration = 0;
		this->velocity = vec2f();
		this->rigth = this->left = this->up = this->down = false;
		this->jump = false;
		this->Size = sizeof(Collider);
	}

public:
	virtual void Update(float delta, Game_Input *input);
	virtual void Init()
	{
		OldPos = &Owner->ObjectTransform.OldPos;
	}
	std::vector<char*> checkObjectCollision();

	bool CheckGroundCollision();
public:
	vec3f *pos;
	vec3f *OldPos;
	vec2f size;
	vec2f velocity;
	vec2f normal;
	vec2f offset;
	float penetration;

	bool ground;
	bool wall;
	bool trigger;
	bool jump;

	bool  rigth, left, down, up;
	
	bool detected;
};
