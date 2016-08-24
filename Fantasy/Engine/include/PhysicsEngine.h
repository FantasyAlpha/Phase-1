#pragma once

#include <Collider.h>

struct CollisionFlags
{
	bool jumpflag = 0;
};

struct Physics
{
	float acceleration;
	float Gravity;
	float mass;
};

struct CollidedPair
{
	char *Name1;
	char *Name2;
};

struct PhysicsEngine
{
public:
	Cinder::Memory::MemoryPool CollidedNamesPool;
	std::vector<CollidedPair *> Colliders;
	Physics ph;
	SceneManager *Scene = 0;

public:
	void Init();

	void Update();

	bool SweptAABBvsAABB(Collider *A, Collider *B, float delta);

	bool AABBvsAABB(Collider *A, Collider *B);
};