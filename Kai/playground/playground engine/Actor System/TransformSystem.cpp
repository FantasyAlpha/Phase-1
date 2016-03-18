#include "TransformSystem.h"

void InitTransformSystem(StackAllocator *sourceAllocator, World *manager)
{
	InitPartialPoolSystem(sourceAllocator, &manager->TransformManager.Data.Allocator, TOTAL_TRANSFORMS_SIZE, TRANSFORM_COMPONENT_SIZE);
	manager->TransformManager.Transforms = (TransformComponent*)manager->TransformManager.Data.Allocator.Blocks->Dimensions.BaseAddress;
}

void AddTransform(World *manager, char *actorName, TransformComponent transform)
{
	uint32 actorIndex = GetActorIndex(manager, actorName);
	if (actorIndex < MAX_ACTOR_COUNT)
	{
		if (!manager->TransformManager.Data.Allocator.Blocks[actorIndex].IsUsed)
		{
			PoolAlloc(&manager->TransformManager.Data.Allocator, actorIndex);

			manager->TransformManager.Transforms[actorIndex] = transform;
		}
	}
}

uint32 GetTransformIndex(World *manager, char *name)
{
	uint32 actorIndex = GetActorIndex(manager, name);
	if (manager->TransformManager.Data.Allocator.Blocks[actorIndex].IsUsed)
	{
		return GetActorIndex(manager, name);
	}

	return MAX_TRANSFORM_COUNT + 1;
}

char* GetTransformOwner(World *manager, uint32 index)
{
	return manager->Actors[index].Name;
}

void RemoveTransformChildren(World *manager, char *name, uint32 index)
{
	if ((*manager->TransformManager.Transforms[index].Children).size() == 0)
	{
		return;
	}

	for (uint32 i = 0; i < (*manager->TransformManager.Transforms[index].Children).size(); i++)
	{
		RemoveTransformChildren(manager, GetTransformOwner(manager, (*manager->TransformManager.Transforms[index].Children)[i]), (*manager->TransformManager.Transforms[index].Children)[i]);
		PoolDealloc(&manager->TransformManager.Data.Allocator, (*manager->TransformManager.Transforms[index].Children)[i], false);
		manager->TransformManager.Transforms[index].Children->erase(manager->TransformManager.Transforms[index].Children->begin() + i);
		i--;
	}
}

void RemoveTransform(World *manager, char *name)
{
	if (manager->TransformManager.Data.Allocator.Blocks[manager->ActiveActorMap[name]].IsUsed)
	{
		uint32 index = GetTransformIndex(manager, name);
		uint32 parentIndex = GetTransformIndex(manager, manager->TransformManager.Transforms[index].ParentName);
		RemoveTransformChildren(manager, name, index);
		PoolDealloc(&manager->TransformManager.Data.Allocator, index, false);
		if (parentIndex < MAX_TRANSFORM_COUNT)
		{
			(*manager->TransformManager.Transforms[parentIndex].Children).erase((*manager->TransformManager.Transforms[parentIndex].Children).begin() + index);
		}
	}
}

void AddTransformChild(World *manager, char *parentActorName, char *childActorName)
{
	if (parentActorName != childActorName 
		&& manager->ActiveActorMap.find(parentActorName) != manager->ActiveActorMap.end() 
		&& manager->ActiveActorMap.find(childActorName) != manager->ActiveActorMap.end() 
		&& manager->TransformManager.Data.Allocator.Blocks[manager->ActiveActorMap[parentActorName]].IsUsed 
		&& manager->TransformManager.Data.Allocator.Blocks[manager->ActiveActorMap[childActorName]].IsUsed)
	{
		uint32 childIndex = GetTransformIndex(manager, childActorName);
		uint32 oldParentIndex = GetTransformIndex(manager, manager->TransformManager.Transforms[childIndex].ParentName);
		uint32 newParentIndex = GetTransformIndex(manager, parentActorName);

		if (oldParentIndex < MAX_TRANSFORM_COUNT)
		{
			uint32 index = MAX_TRANSFORM_COUNT + 1;
			for (uint32 i = 0; i < (*manager->TransformManager.Transforms[oldParentIndex].Children).size(); i++)
			{
				if ((*manager->TransformManager.Transforms[oldParentIndex].Children)[i] == childIndex)
				{
					index = i;
					break;
				}
			}

			(*manager->TransformManager.Transforms[oldParentIndex].Children).erase((*manager->TransformManager.Transforms[oldParentIndex].Children).begin() + index);
		}

		if (manager->TransformManager.Transforms[newParentIndex].ParentName != childActorName)
		{
			manager->TransformManager.Transforms[childIndex].ParentName = parentActorName;
			manager->TransformManager.Transforms[childIndex].ParentIndex = manager->ActiveActorMap[parentActorName];
			(*manager->TransformManager.Transforms[newParentIndex].Children).push_back(childIndex);
		}
	}	
}

void SetMainParent(World *manager, char *name)
{
	if (manager->ActiveActorMap.find(name) != manager->ActiveActorMap.end() && manager->TransformManager.Data.Allocator.Blocks[manager->ActiveActorMap[name]].IsUsed)
	{
		if (!manager->TransformManager.MainParent)
		{
			manager->TransformManager.MainParent = &manager->TransformManager.Transforms[manager->ActiveActorMap[name]];
		}
	}
}

void UpdateTransformSystem(World *manager)
{
	UpdateTransformChildren(manager, manager->TransformManager.MainParent, false);
	UpdateOldTransform(manager, manager->TransformManager.MainParent);
}

void UpdateTransformChildren(World *manager, TransformComponent *transform, bool parentChanged)
{
	if ((*transform->Children).size() == 0)
	{
		if ((!parentChanged || transform->ParentIndex > MAX_TRANSFORM_COUNT)
			&& (transform->OldTransform.Position != transform->CurrentTransform.Position
			|| transform->OldTransform.Rotation != transform->CurrentTransform.Rotation
			|| transform->OldTransform.Scale != transform->CurrentTransform.Scale))
		{
			transform->ModelMatrix = ModelMatrix(&transform->CurrentTransform);
			if (transform->ParentIndex < MAX_TRANSFORM_COUNT)
			{
				transform->ModelMatrix = manager->TransformManager.Transforms[transform->ParentIndex].ModelMatrix * transform->ModelMatrix;
			}
		}
		else if (parentChanged)
		{
			transform->ModelMatrix = manager->TransformManager.Transforms[transform->ParentIndex].ModelMatrix * ModelMatrix(&transform->CurrentTransform);
		}
	}

	for (uint32 i = 0; i < transform->Children->size(); i++)
	{
		bool didChange = false;		

		if ((!parentChanged || transform->ParentIndex > MAX_TRANSFORM_COUNT)
			&& (transform->OldTransform.Position != transform->CurrentTransform.Position
			|| transform->OldTransform.Rotation != transform->CurrentTransform.Rotation
			|| transform->OldTransform.Scale != transform->CurrentTransform.Scale))
		{
			if (i == 0)
			{
				transform->ModelMatrix = ModelMatrix(&transform->CurrentTransform);
				if (transform->ParentIndex < MAX_TRANSFORM_COUNT)
				{
					transform->ModelMatrix = manager->TransformManager.Transforms[transform->ParentIndex].ModelMatrix * transform->ModelMatrix;
				}
			}

			didChange = true;
		}
		else if (parentChanged)
		{
			if (i == 0)
			{
				transform->ModelMatrix = manager->TransformManager.Transforms[manager->ActiveActorMap[transform->ParentName]].ModelMatrix * ModelMatrix(&transform->CurrentTransform);
			}

			didChange = true;
		}

		UpdateTransformChildren(manager, &manager->TransformManager.Transforms[(*transform->Children)[i]], didChange);
	}
}

void UpdateOldTransform(World *manager, TransformComponent *transform)
{
	for (uint32 i = 0; i < transform->Children->size(); i++)
	{
		if (i == 0)
		{
			transform->OldTransform = transform->CurrentTransform;
		}

		UpdateOldTransform(manager, &manager->TransformManager.Transforms[(*transform->Children)[i]]);
	}

	if (transform->Children->size() == 0)
	{
		transform->OldTransform = transform->CurrentTransform;
	}
}


Transform* GetTransform(World *manager, char *name)
{
	uint32 index = GetTransformIndex(manager, name);

	if (index < MAX_TRANSFORM_COUNT)
	{
		return &manager->TransformManager.Transforms[index].CurrentTransform;
	}

	return NULL;
}

