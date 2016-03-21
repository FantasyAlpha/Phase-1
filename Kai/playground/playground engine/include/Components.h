#pragma once

#include <unordered_map>
#include <vector>
#include <MemoryAllocator.h>
#include <Mesh.h>
#include <Material.h>
#include <Shader.h>
#include <Transform.h>
#include <Types.h>

#define MAX_ACTOR_COUNT 100
#define MAX_SPRITE_COUNT 100
#define MAX_TRANSFORM_COUNT MAX_ACTOR_COUNT

struct World;

struct ComponentData
{
	PoolAllocator Allocator;
};

struct Renderer
{
	Mesh Renderable;
	Material RenderableMaterial;
};

#define SPRITE_RENDERER_SIZE sizeof(Renderer)
#define TOTAL_SPRITES_ALLOCATOR_SIZE ((SPRITE_RENDERER_SIZE + sizeof(PoolBlock)) * MAX_SPRITE_COUNT) + sizeof(PoolAllocator)
#define TOTAL_SPRITES_SIZE SPRITE_RENDERER_SIZE * MAX_SPRITE_COUNT

struct RendererSystem
{
	PoolAllocator Allocator;
	Renderer *Renderers;
	Shader MainShader;
	uint32 MaxSize;
	World *Owner;

	void InitRendererSystem(StackAllocator *sourceAllocator);

	void AddComponent(char *actorName, Renderer renderer);

	uint32 GetRendererIndex(char *name);

	void RemoveRenderer(char *actorName);

	void RenderAllActive();

	void InitMainShader(char *vertexShader, char *fragmentShader);
};

struct TransformComponent
{
	Transform CurrentTransform;
	Transform OldTransform;

	mat4 ModelMatrix;

	char *ParentName;
	uint32 ParentIndex;

	std::vector<uint32> *Children;
};
 
#define TRANSFORM_COMPONENT_SIZE sizeof(TransformComponent)
#define TOTAL_TRANSFORMS_ALLOCATOR_SIZE ((TRANSFORM_COMPONENT_SIZE + sizeof(PoolBlock)) * MAX_TRANSFORM_COUNT) + sizeof(PoolAllocator)
#define TOTAL_TRANSFORMS_SIZE TRANSFORM_COMPONENT_SIZE * MAX_TRANSFORM_COUNT

struct TransformSystem
{
	PoolAllocator Allocator;
	TransformComponent *Transforms;
	TransformComponent *MainParent;
	uint32 MainParentIndex;
	uint32 MaxSize;
	World *Owner;

	void InitTransformSystem(StackAllocator *sourceAllocator);

	void AddComponent(char *actorName, TransformComponent transform);

	uint32 GetTransformIndex(char *name);

	char* GetTransformOwner(uint32 index);

	void RemoveTransformChildren(char *name, uint32 index);

	void RemoveTransform(char *name);

	void AttachTransformChild(char *parentActorName, char *childActorName);

	void SetMainParent(char *name);

	void UpdateTransformSystem();

	void UpdateTransformChildren(TransformComponent *transform, bool parentChanged);

	void UpdateOldTransform(TransformComponent *transform);

	Transform* GetTransform(char *name);
};

#define ACTOR_SIZE sizeof(Actor)
#define TOTAL_ACTORS_ALLOCATOR_SIZE ((ACTOR_SIZE + sizeof(PoolBlock)) * MAX_ACTOR_COUNT) + sizeof(PoolAllocator)
#define TOTAL_ACTORS_SIZE ACTOR_SIZE * MAX_ACTOR_COUNT

struct Actor
{
	char *Name;
	uint32 ID;
};

struct ActorSystem
{
	PoolAllocator Allocator;
	Actor *Actors;
	uint32 ActorCount;
	uint32 MaxSize;
	World *Owner;
	std::unordered_map<char*, uint32> ActiveActorMap;

	void InitActorSystem(StackAllocator *sourceAllocator);

	Actor* CreateActor(char *name);

	Actor* GetActor(char *name);

	uint32 GetActorIndex(char *name);

	void DestroyActor(char *name);
};

