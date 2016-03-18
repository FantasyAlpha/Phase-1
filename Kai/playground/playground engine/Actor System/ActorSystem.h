#pragma once

#include "..\Memory\MemoryAllocator.h"
#include <vector>
#include <unordered_map>
#include "Components.h"

enum ComponentType
{
	SPRITE_RENDERER_COMPONENT = 0,
	TRANSFORM_COMPONENT = 1,
};

struct Actor
{
	char *Name;
	uint32 ID;
};

#define ACTOR_SIZE sizeof(Actor)
#define TOTAL_ACTORS_SIZE ACTOR_SIZE * MAX_ACTOR_COUNT

struct World
{
	PoolAllocator Allocator;
	Actor *Actors;
	uint32 ActorCount;
	std::unordered_map<char*, uint32> ActiveActorMap;
	SpriteRendererSystem RendererSystem;
	TransformSystem TransformManager;
	Camera Cam;
};

void InitWorld(StackAllocator *sourceAllocator, World *manager);

Actor* CreateActor(World *manager, char *name);

Actor* GetActor(World *manager, char *name);

uint32 GetActorIndex(World *manager, char *name);

void DestroyActor(World *manager, char *name);



