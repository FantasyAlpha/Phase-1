#include "Actor System\ActorSystem.h"
#include "Actor System\SpriteRenderingSystem.h"
#include "Actor System\TransformSystem.h"

void InitWorld(StackAllocator *sourceAllocator, World *manager)
{
	InitPartialPoolSystem(sourceAllocator, &manager->Allocator, TOTAL_ACTORS_SIZE, ACTOR_SIZE);
	manager->Actors = (Actor*)manager->Allocator.Blocks->Dimensions.BaseAddress;
}

Actor* CreateActor(World *manager, char *name)
{
	PoolBlock block = PoolAlloc(&manager->Allocator);

	manager->ActiveActorMap[name] = block.BlockIndex;
	manager->ActorCount++;

	manager->Actors[block.BlockIndex].ID = block.BlockIndex;
	manager->Actors[block.BlockIndex].Name = name;

	return &manager->Actors[block.BlockIndex];
}

Actor* GetActor(World *manager, char *name)
{
	if (manager->ActiveActorMap.find(name) != manager->ActiveActorMap.end())
	{
		return &manager->Actors[manager->ActiveActorMap[name]];
	}

	return NULL;
}

uint32 GetActorIndex(World *manager, char *name)
{
	if (manager->ActiveActorMap.find(name) != manager->ActiveActorMap.end())
	{
		return manager->ActiveActorMap[name];
	}

	return MAX_ACTOR_COUNT + 1;
}

void DestroyActor(World *manager, char *name)
{
	if (manager->ActiveActorMap.find(name) != manager->ActiveActorMap.end())
	{
		uint32 actorIndex = manager->ActiveActorMap[name];
		
		if (GetRendererIndex(manager, name) < MAX_SPRITE_COUNT)
		{
			RemoveRenderer(manager, name);
		}

		if (GetTransformIndex(manager, name) < MAX_TRANSFORM_COUNT)
		{
			RemoveTransform(manager, name);
		}

		PoolDealloc(&manager->Allocator, actorIndex);
		manager->ActiveActorMap.erase(name);
		manager->ActorCount--;
	}
}

