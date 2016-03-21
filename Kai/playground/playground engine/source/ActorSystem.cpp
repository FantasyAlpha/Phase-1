#include <Components.h>
#include <World.h>

void ActorSystem::InitActorSystem(StackAllocator *sourceAllocator)
{
	MaxSize = TOTAL_ACTORS_SIZE;
	InitPartialPoolSystem(sourceAllocator, &this->Allocator, MaxSize, ACTOR_SIZE);
	this->Actors = (Actor*)(this->Allocator.Blocks)->Dimensions.BaseAddress;
}

Actor* ActorSystem::CreateActor(char *name)
{
	PoolBlock block = PoolAlloc(&this->Allocator);

	this->ActiveActorMap[name] = block.BlockIndex;
	this->ActorCount++;

	this->Actors[block.BlockIndex].ID = block.BlockIndex;
	this->Actors[block.BlockIndex].Name = name;

	return &this->Actors[block.BlockIndex];
}

Actor* ActorSystem::GetActor(char *name)
{
	if (this->ActiveActorMap.find(name) != this->ActiveActorMap.end())
	{
		return &this->Actors[this->ActiveActorMap[name]];
	}

	return NULL;
}

uint32 ActorSystem::GetActorIndex(char *name)
{
	if (this->ActiveActorMap.find(name) != this->ActiveActorMap.end())
	{
		return this->ActiveActorMap[name];
	}

	return MAX_ACTOR_COUNT + 1;
}

void ActorSystem::DestroyActor(char *name)
{
	if (this->ActiveActorMap.find(name) != this->ActiveActorMap.end())
	{
		uint32 actorIndex = this->ActiveActorMap[name];
		
		if (Owner->RendererManager.GetRendererIndex(name) < MAX_SPRITE_COUNT)
		{
			Owner->RendererManager.RemoveRenderer(name);
		}

		if (Owner->TransformManager.GetTransformIndex(name) < MAX_TRANSFORM_COUNT)
		{
			Owner->TransformManager.RemoveTransform(name);
		}

		PoolDealloc(&this->Allocator, actorIndex);
		this->ActiveActorMap.erase(name);
		this->ActorCount--;
	}
}

