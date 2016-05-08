#include <Components.h>
#include <World.h>
#include <Collision.h>


void CollisionSystem::InitCollisionSystem(StackAllocator *allocator)
{
	InitPartialPoolSystem(allocator, &Allocator, TOTAL_COLLIDER_SIZE, COLLIDER_SIZE);
	Colliders = (Collider*)Allocator.Blocks->Dimensions.BaseAddress;
}

void CollisionSystem::AddComponent(char *name, vec3f *pos, vec2f size, vec2f velocity, bool wall, bool trigger)
{
	uint32 transformIndex = Owner->TransformManager.GetTransformIndex(name);
	//************
	/*collider.OwnerIndex = actorIndex;
	collider.pos=Owner->TransformManager.Transforms[collider.OwnerIndex].CurrentTransform.Position;*/

	if (transformIndex < MAX_TRANSFORM_COUNT)
	{
		if (!Allocator.Blocks[transformIndex].IsUsed)
		{
			PoolBlock block = PoolAlloc(&Allocator);

			//Colliders[block.BlockIndex] = collider;
			Colliders[block.BlockIndex].OwnerIndex = transformIndex;
			Colliders[block.BlockIndex].OwnerName = name;
			//******************** like an intialize
			Colliders[block.BlockIndex].pos = pos;
			Colliders[block.BlockIndex].size = size;
			Colliders[block.BlockIndex].velocity = velocity;

			Colliders[block.BlockIndex].wall = wall;
			Colliders[block.BlockIndex].trigger = trigger;


			Owner_ComponentMap[name] = block.BlockIndex;
		}
	}
}

void CollisionSystem::RemoveComponent(char *name)
{
	uint32 index = GetColliderIndex(name);
	if (index < MAX_ACTOR_COUNT)
	{
		Owner_ComponentMap.erase(name);
		PoolDealloc(&Allocator, index);
	}
}

void CollisionSystem::UpdateCollisionSystem(float delta)
{
	uint32 index1 = Allocator.FirstUsed;
	uint32 index2 = Allocator.FirstUsed;

	//index1 = Allocator.FirstUsed;
	while (index1 != Allocator.BlockCount + 1)
	{
		float temp1, temp2;

		temp1 = Owner->TransformManager.Transforms[Colliders[index1].OwnerIndex].CurrentTransform.Position.X;
		temp2 = Owner->TransformManager.Transforms[Colliders[index1].OwnerIndex].CurrentTransform.Position.Y;

		Colliders[index1].pos->X = temp1;
		Colliders[index1].pos->Y = temp2;

		//  Colliders[index1].pos;
		index1 = Allocator.Blocks[index1].NextUsedIndex;
	}

	index1 = Allocator.FirstUsed;

	while (index1 != Allocator.BlockCount + 1)
	{

		Colliders[index1].rigth = Colliders[index1].left = Colliders[index1].down = Colliders[index1].up = false;
		index1 = Allocator.Blocks[index1].NextUsedIndex;

	}
	index1 = Allocator.FirstUsed;

	while (index1 != Allocator.BlockCount + 1)
	{

		while (index2 != Allocator.BlockCount + 1)
		{

			SweptAABBvsAABB(&Colliders[index1], &Colliders[index2], delta);

			index2 = Allocator.Blocks[index2].NextUsedIndex;
		}
		//***************		

		index1 = Allocator.Blocks[index1].NextUsedIndex;
		index2 = Allocator.FirstUsed;
	}

	index1 = Allocator.FirstUsed;
	while (index1 != Allocator.BlockCount + 1)
	{
		float temp1, temp2;
		temp1 = Colliders[index1].pos->X;
		temp2 = Colliders[index1].pos->Y;

		Owner->TransformManager.Transforms[Colliders[index1].OwnerIndex].CurrentTransform.Position.X= temp1;
		Owner->TransformManager.Transforms[Colliders[index1].OwnerIndex].CurrentTransform.Position.Y = temp2;

		index1 = Allocator.Blocks[index1].NextUsedIndex;
	}
}

uint32 CollisionSystem::GetColliderIndex(char *name)
{
	uint32 actorIndex = Owner->ActorManager.GetActorIndex(name);

	if (Owner->ActorManager.Allocator.Blocks[actorIndex].IsUsed)
	{
		if (Owner_ComponentMap.find(name) != Owner_ComponentMap.end())
		{
			return Owner_ComponentMap[name];
		}
	}

	return MAX_ACTOR_COUNT + 1;
}

Collider* CollisionSystem::GetCollider(char *name)
{
	uint32 index = GetColliderIndex(name);

	if (index < MAX_ACTOR_COUNT)
	{
		return &Colliders[index];
	}

	return NULL;
}