#pragma once

#include <GameComponent.h>
#include <SceneManager.h>

struct Move
	: public GameComponent
{
public:
	Move(char *name, float speed, float acceleration, Collider *collider)
	{
		this->Name = name;
		this->Type = "MOVE";
		this->Speed = speed;
		this->Acceleration = acceleration;
		this->Size = sizeof(Move);

		this->collider = collider;
	}

public:
	void SetKeys(int up, int down, int left, int right)
	{
		Left = Scene->GetInputKey(left);
		Right = Scene->GetInputKey(right);
		Up = Scene->GetInputKey(up);
		Down = Scene->GetInputKey(down);
	}

	virtual void PreInit()
	{
		SetKeys(VK_UP, VK_DOWN, VK_LEFT, VK_RIGHT);
	}

	virtual void Init(){}
	virtual void Destroy(){}
	virtual void Update(float delta, Game_Input *input);
	virtual void Render(){}

private:
	float Speed;
	float Acceleration;
	vec3f Velocity;

	Collider *collider;
	uint32 Up, Down, Left, Right;
};