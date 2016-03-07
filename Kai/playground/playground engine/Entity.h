#pragma once

#include "MemoryAllocator.h"
#include "Mesh.h"

enum ComponentID
{
	NO_COMPONENT = 0x00000000,
	SPRITE_RENDERER_COMPONENT = 0x00000001,
	NAME_COMPONENT = 0x00000002
};

struct NameComponentSystem
{
	// 1- Stack allocator (give fixed size)
	StackAllocator EntityAllocator;
	// 2- Names
	char **Names;
};

typedef uint32 EntityID;
struct EntitySystem
{
	// 1- Stack allocator (give fixed size)
	StackAllocator EntityAllocator;
	// 2- Components Array
	uint32 *Components;
	// 3- Used Count
	uint32 UsedCount;
	// 4- Total Count
	uint32 TotalCount;
};

struct SpriteRenderer
{
	// 1- Mesh 
	Mesh Sprite;
	// 2- Material 
	Material Material;
};

struct SpriteRendererSystem
{
	// 1- Stack allocator (give fixed size)
	StackAllocator EntityAllocator;
	// 2- Sprite renderers
	SpriteRenderer *Renderers;
	// 3- Used Count
	uint32 UsedCount;
	// 4- Total Count
	uint32 TotalCount;
};

struct World
{
	StackAllocator WorldAllocator;
	EntitySystem WorldEntities;
	NameComponentSystem NamesSystem;
	SpriteRendererSystem SpriteRenderer;
};

void InitWorld(World *world, MainAllocator *sourceAllocator, uint32 size);
void ResetWorld(World *world);

void InitEntitySystem(World *world, uint32 size);
EntityID CreateEntity(World *world, char *name);
bool EntityIsAlive(World *world, EntityID id);
void DestroyEntity(World *world, EntityID id);

void InitNameSystem(World *world, uint32 size);

void InitSpriteRendererSystem(World *world, uint32 size);

void RegisterSpriteRenderer(World *world, EntityID id, SpriteRenderer spriteRenderer);

void RemoveSpriteRenderer(World *world, EntityID id);

void RenderSpriteRendererSystem(World *world);
