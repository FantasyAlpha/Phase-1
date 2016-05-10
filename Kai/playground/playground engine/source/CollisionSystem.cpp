#include <Components.h>
#include <Collision.h>


void CollisionSystem::InitCollisionSystem(uint32 count)
{
	Pool = Cinder::Memory::MemoryPool(sizeof(Collider), count, false);
	Colliders = (Collider*)Pool.Elements;
}

void CollisionSystem::AddComponent(char *name, vec3f *pos, vec2f size, vec2f velocity, bool wall, bool trigger)
{
	uint32 transformIndex = Owner->ActorManager.GetActorIndex(name);
	//************

	if (transformIndex < Pool.ChunkCount)
	{
		if (Pool.UsedHeaders[transformIndex].Current == Pool.ChunkCount + 1)
		{
			Cinder::Memory::PoolHeader block = Pool.Alloc();

			//Colliders[block.BlockIndex] = collider;
			Colliders[block.Current].OwnerIndex = transformIndex;
			Colliders[block.Current].OwnerName = name;
			//******************** like an intialize
			Colliders[block.Current].pos = pos;
			Colliders[block.Current].size = size;
			Colliders[block.Current].velocity = velocity;

			Colliders[block.Current].wall = wall;
			Colliders[block.Current].trigger = trigger;


			Owner_ComponentMap[name] = block.Current;
		}
	}
}

void CollisionSystem::RemoveComponent(char *name)
{
	uint32 index = GetColliderIndex(name);
	if (index < Pool.ChunkCount)
	{
		Owner_ComponentMap.erase(name);
		Pool.Dealloc(index);
	}
}

void CollisionSystem::UpdateCollisionSystem(float delta)
{
	uint32 index1 = Pool.FirstUsed;
	uint32 index2 = Pool.FirstUsed;

	while (index1 != Pool.ChunkCount + 1)
	{

		Colliders[index1].rigth = Colliders[index1].left = Colliders[index1].down = Colliders[index1].up = false;
		index1 = Pool.UsedHeaders[index1].Next;
	}

	index1 = Pool.FirstUsed;

	while (index1 != Pool.ChunkCount + 1)
	{

		while (index2 != Pool.ChunkCount + 1)
		{

			SweptAABBvsAABB(&Colliders[index1], &Colliders[index2], delta);

			index2 = Pool.UsedHeaders[index2].Next;
		}
		//***************		

		index1 = Pool.UsedHeaders[index1].Next;
		index2 = Pool.FirstUsed;
	}
}

uint32 CollisionSystem::GetColliderIndex(char *name)
{
	uint32 actorIndex = Owner->ActorManager.GetActorIndex(name);

	if (Owner->ActorManager.Pool.UsedHeaders[actorIndex].Current != Owner->ActorManager.Pool.ChunkCount + 1)
	{
		if (Owner_ComponentMap.find(name) != Owner_ComponentMap.end())
		{
			return Owner_ComponentMap[name];
		}
	}

	return Pool.ChunkCount + 1;
}

Collider* CollisionSystem::GetCollider(char *name)
{
	uint32 index = GetColliderIndex(name);

	if (index < Pool.ChunkCount)
	{
		return &Colliders[index];
	}

	return NULL;
}