#pragma once

#include <unordered_map>
#include <vector>
#include <MemoryAllocator.h>
#include <Texture.h>
#include <Mesh.h>
#include <Shader.h>
#include <Transform.h>
#include <Types.h>

#define MAX_ACTOR_COUNT 100
#define MAX_CLIP_COUNT 100
#define MAX_SPRITE_COUNT 100
#define MAX_TRANSFORM_COUNT MAX_ACTOR_COUNT

struct World;

struct Renderable
{
	vec3 Position;
	vec2 Size;

	Material RenderableMaterial;

	AnimationClip *Clip;

	uint32 OwnerIndex;

	bool WithClip;
};

struct BatchRenderer
{
	MeshBatch RenderableBatch;
	Material RenderableMaterial;

	uint32 OwnerIndex;
};

#define SPRITE_RENDERER_SIZE sizeof(Renderer)
#define TOTAL_SPRITES_ALLOCATOR_SIZE ((SPRITE_RENDERER_SIZE + sizeof(PoolBlock)) * MAX_SPRITE_COUNT)
#define TOTAL_SPRITES_SIZE SPRITE_RENDERER_SIZE * MAX_SPRITE_COUNT

struct RendererSystem
{
	PoolAllocator Allocator;
	Renderable *Renderables;
	BatchRenderer Renderer;
	BatchRenderer DebugRenderer;
	Shader MainShader;
	Shader DebugShader;
	uint32 MaxSize;
	World *Owner;
	std::unordered_map<char*, uint32> Owner_ComponentMap;

	void InitRendererSystem(StackAllocator *sourceAllocator);

	void AddComponent(char *actorName, vec3 pos, vec2 size, Material material, AnimationClip *clip = 0);

	uint32 GetRenderableIndex(char *name);
	Renderable* GetRenderable(char *name);

	void RemoveRenderer(char *actorName);

	void RenderAllActive();
	void RenderDebugShapes();

	void InitMainShader(char *vertexShader, char *fragmentShader);
	void InitDebugShader(char *vertexShader, char *fragmentShader);
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
#define TOTAL_TRANSFORMS_ALLOCATOR_SIZE ((TRANSFORM_COMPONENT_SIZE + sizeof(PoolBlock)) * MAX_TRANSFORM_COUNT)
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

struct AnimationClip
{
	uint32 MaxCountHorizontal;
	uint32 MaxCountVertical;

	uint32 *Indices;
	uint32 FrameCount;

	float FrameWidth;
	float FrameHeight;

	float RunSpeed_FPS;
	float TimeElapsed;
	float Delta;

	uint32 Counter;

	char *Name;
	bool Loop;
};

#define CLIP_SIZE sizeof(AnimationClip)
#define TOTAL_CLIPS_ALLOCATOR_SIZE ((CLIP_SIZE + sizeof(PoolBlock)) * MAX_CLIP_COUNT)
#define TOTAL_CLIPS_SIZE CLIP_SIZE * MAX_CLIP_COUNT

struct AnimationSystem
{
	PoolAllocator Allocator;
	AnimationClip *Clips;
	World *Owner;
	uint32 MaxSize;

	std::unordered_map<char*, uint32> Name_ComponentMap;

	void InitAnimationSystem(StackAllocator *sourceAllocator);

	void AddComponent(char *name, uint32 maxCountHorizontal, uint32 maxCountVertical, uint32 *indices
		, uint32 frameCount, float runSpeed_FPS, bool loop);

	uint32 GetAnimationClipIndex(char *name);
	AnimationClip* GetAnimationClip(char *name);

	void RemoveAnimationClip(char *name);

	void SwitchAnimation(char *actor, char *animation);
};

#define ACTOR_SIZE sizeof(Actor)
#define TOTAL_ACTORS_ALLOCATOR_SIZE ((ACTOR_SIZE + sizeof(PoolBlock)) * MAX_ACTOR_COUNT)
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

