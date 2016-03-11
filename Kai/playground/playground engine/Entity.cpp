#include "Entity.h"

void InitEntitySystem(World *world, uint32 size)
{
	uint32 totalSize = size * (sizeof(EntityID));

	if (InitPartialStackSystem(&world->WorldAllocator, &world->WorldEntities.EntityAllocator, totalSize) == AllocatorErrors::NO_ERRORS)
	{
		StackBookmark bookmark = StackAlloc(&world->WorldEntities.EntityAllocator, world->WorldEntities.EntityAllocator.TotalSize);
		world->WorldEntities.Components = (uint32*)bookmark.Bookmark;

		for (uint32 i = 0; i < size; i++)
		{
			world->WorldEntities.Components[i] = ComponentID::NO_COMPONENT;
		}

		world->WorldEntities.TotalCount = size;
		world->WorldEntities.UsedCount = 0;
	}
}

EntityID CreateEntity(World *world, char *name)
{
	for (uint32 i = 0; i < world->WorldEntities.TotalCount; i++)
	{
		if (world->WorldEntities.Components[i] == ComponentID::NO_COMPONENT)
		{
			world->WorldEntities.UsedCount++;
			world->NamesSystem.Names[i] = name;
			world->WorldEntities.Components[i] = ComponentID::NAME_COMPONENT;
			return i;
		}
	}

	return world->WorldEntities.TotalCount + 1;
}

bool EntityIsAlive(World *world, EntityID id)
{
	return world->WorldEntities.Components[id] != ComponentID::NO_COMPONENT;
}

void DestroyEntity(World *world, EntityID id)
{
	if (EntityIsAlive(world, id))
	{
		world->WorldEntities.UsedCount--;
		world->WorldEntities.Components[id] = ComponentID::NO_COMPONENT;
		world->NamesSystem.Names[id] = 0;
	}
}

void InitNameSystem(World *world, uint32 size)
{
	uint32 totalSize = size * (sizeof(char**));

	if (InitPartialStackSystem(&world->WorldAllocator, &world->NamesSystem.EntityAllocator, totalSize) == AllocatorErrors::NO_ERRORS)
	{
		world->NamesSystem.Names = (char**)world->NamesSystem.EntityAllocator.StackPointer;
		StackBookmark bookmark = StackAlloc(&world->NamesSystem.EntityAllocator, world->NamesSystem.EntityAllocator.TotalSize);

		world->NamesSystem.Names = (char**)bookmark.Bookmark;
	}
}

void InitWorld(World *world, MainAllocator *sourceAllocator, uint32 size)
{
	uint32 totalSize = size * (sizeof(EntityID) + sizeof(char**) + sizeof(SpriteRenderer));

	if (sourceAllocator->MainSystem == AllocatorTypes::FRAME_ALLOCATOR)
	{
		if (InitPartialStackSystem(sourceAllocator->FrameSystem, &world->WorldAllocator, HeapType::LOWER_HEAP, totalSize) == AllocatorErrors::NO_ERRORS)
		{
			InitEntitySystem(world, size);
			InitNameSystem(world, size);
			InitSpriteRendererSystem(world, size);
		}
	}
	else if (sourceAllocator->MainSystem == AllocatorTypes::STACK_ALLOCATOR)
	{
		if (InitPartialStackSystem(sourceAllocator->StackSystem, &world->WorldAllocator, totalSize) == AllocatorErrors::NO_ERRORS)
		{
			InitEntitySystem(world, size);
			InitNameSystem(world, size);
			InitSpriteRendererSystem(world, size);
		}
	} 
	else
	{
		if (InitPartialStackSystem(sourceAllocator->PoolSystem, &world->WorldAllocator) == AllocatorErrors::NO_ERRORS)
		{
			InitEntitySystem(world, size);
			InitNameSystem(world, size);
			InitSpriteRendererSystem(world, size);
		}
	}
}

void ResetWorld(World *world)
{
	StackBookmark entitySystemReset = { world->WorldEntities.EntityAllocator.Dimensions.BaseAddress };
	StackBookmark namesSystemReset = { world->NamesSystem.EntityAllocator.Dimensions.BaseAddress };
	StackBookmark rendererSystemReset = { world->SpriteRenderer.EntityAllocator.Dimensions.BaseAddress };
	StackBookmark worldReset = { world->WorldAllocator.Dimensions.BaseAddress };

	StackRelease(&world->SpriteRenderer.EntityAllocator, &rendererSystemReset);
	StackRelease(&world->NamesSystem.EntityAllocator, &namesSystemReset);
	StackRelease(&world->WorldEntities.EntityAllocator, &entitySystemReset);
	StackRelease(&world->WorldAllocator, &worldReset);
}

void InitSpriteRendererSystem(World *world, uint32 size)
{
	uint32 totalSize = size * (sizeof(SpriteRenderer));

	if (InitPartialStackSystem(&world->WorldAllocator, &world->SpriteRenderer.EntityAllocator, totalSize) == AllocatorErrors::NO_ERRORS)
	{
		world->SpriteRenderer.Renderers = (SpriteRenderer*)world->SpriteRenderer.EntityAllocator.StackPointer;
		world->SpriteRenderer.TotalCount = size;
	}
}

void RegisterSpriteRenderer(World *world, EntityID id, SpriteRenderer spriteRenderer)
{
	if ((world->WorldEntities.Components[id] & ~(ComponentID::NAME_COMPONENT)) != ComponentID::SPRITE_RENDERER_COMPONENT)
	{
		world->WorldEntities.Components[id] |= ComponentID::SPRITE_RENDERER_COMPONENT;
		world->SpriteRenderer.Renderers[world->SpriteRenderer.UsedCount++] = spriteRenderer;
	} 
	else
	{
		world->SpriteRenderer.Renderers[world->SpriteRenderer.UsedCount - 1] = spriteRenderer;
	}
}

void RemoveSpriteRenderer(World *world, EntityID id)
{
	for (uint32 i = id; i < world->SpriteRenderer.UsedCount - 1; i++)
	{
		SpriteRenderer tempSR = world->SpriteRenderer.Renderers[i + 1];

		world->SpriteRenderer.Renderers[i] = tempSR;
	}

	world->SpriteRenderer.UsedCount--;
}

void RenderSpriteRendererSystem(World *world)
{
	for (uint32 i = 0; i < world->SpriteRenderer.TotalCount; i++)
	{
		if ((world->WorldEntities.Components[i] & ~(ComponentID::NAME_COMPONENT)) == ComponentID::SPRITE_RENDERER_COMPONENT)
		{
			BindTexture(&world->SpriteRenderer.Renderers[i].Material.Tex);
			DrawMesh(&world->SpriteRenderer.Renderers[i].Sprite);
			UnbindTexture();
		}
	}
}