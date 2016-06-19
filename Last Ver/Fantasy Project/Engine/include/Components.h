#pragma once

#include <map>
#include <vector>
#include <MemoryAllocator.h>
#include <Cinder\CinderMemory.h>
#include <Texture.h>
#include <Mesh.h>
#include <Shader.h>
#include <Transform.h>
#include <Types.h>

struct SceneManager;

struct Physics
{
	float acceleration;
	float Gravity;
	float mass;
};

struct CollidedPair
{
	char *Name1;
	char *Name2;
};
struct Collider
{
	vec3f *pos;
	vec2f size;
	vec2f velocity;
	bool ground;
	bool wall;
	bool trigger;
	//**************
	bool jump;
	vec2f normal;
	float penetration;

	uint32 OwnerIndex;
	char *OwnerName;

	bool detected;
	bool  rigth, left, down, up;
};

struct CollisionSystem
{
	Cinder::Memory::MemoryPool Pool;
	Cinder::Memory::MemoryPool CollidedNamesPool;
	CollidedPair *CollidedNames;
	Collider *Colliders;
	SceneManager *Owner;
	std::map<char*, uint32> Owner_ComponentMap;
	Physics physics;

	void InitCollisionSystem(uint32 count);
	void AddComponent(char *name, vec3f *pos, vec2f size, bool ground, bool wall, bool trigger);
	void RemoveComponent(char *name);
	void UpdateCollisionSystem(float delta);
	uint32 GetColliderIndex(char *name);
	Collider* GetCollider(char *name);
	bool CheckGroundCollision(char*name);
	std::vector <char*> checkActorCollision(char* ActorName);
};


enum RenderableType
{
	Static,
	Movable
};

struct Renderable
{
	Mesh Buffers;
	Mesh DebugBuffers;

	vec3f Position;
	vec2f Size;

	Material RenderableMaterial;

	AnimationClip *Clip;

	uint32 OwnerIndex;

	bool WithClip;
};

struct BasicLight
{
	vec3f Position;
	vec4f Color;
	float Strength;
};

struct RendererSystem
{
	Cinder::Memory::MemoryPool StaticsPool;
	Cinder::Memory::MemoryPool MovablesPool;

	Renderable *StaticRenderables;
	Renderable *MovableRenderables;

	MeshBatch Renderer;
	MeshBatch DebugRenderer;

	Shader MainShader;
	Shader DebugShader;

	SceneManager *Owner;

	BasicLight MainLight;

	std::map<char*, uint32> Owner_StaticComponentMap;
	std::map<char*, uint32> Owner_MovableComponentMap;

	void InitRendererSystem(uint32 count);

	void AddComponent(char *name, RenderableType type, vec3f pos, vec2f size, Material material, AnimationClip *clip = 0);

	uint32 GetRenderableIndex(char *name, RenderableType type);
	Renderable* GetRenderable(char *name, RenderableType type);

	void RemoveRenderer(char *actorName, RenderableType type);

	void RenderAllActive();
	void RenderDebugShapes();
	void UpdateRenderables();

	void InitMainShader(char *vertexShader, char *fragmentShader);
	void InitDebugShader(char *vertexShader, char *fragmentShader);
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
	float UPS;

	uint32 Counter;

	char *Name;
	bool Loop;
};

struct AnimationSystem
{
	Cinder::Memory::MemoryPool Pool;
	AnimationClip *Clips;
	SceneManager *Owner;
	uint32 MaxSize;

	std::map<char*, uint32> Name_ComponentMap;

	void InitAnimationSystem(uint32 count);

	void AddComponent(char *name, uint32 maxCountHorizontal, uint32 maxCountVertical, uint32 *indices
		, uint32 frameCount, float runSpeed, bool loop);

	uint32 GetAnimationClipIndex(char *name);
	AnimationClip* GetAnimationClip(char *name);

	void RemoveAnimationClip(char *name);

	void SwitchAnimation(char *actor, RenderableType type, char *animation, Texture *texture = NULL);
};

struct Actor
{
	std::vector<uint32> *Children;
	Transform CurrentTransform;
	Transform OldTransform;

	mat4f ModelMatrix;

	uint32 ParentIndex;

	char *ParentName;
	char *Name;
	char *Tag;
};

struct ActorSystem
{
	Cinder::Memory::MemoryPool Pool;
	Actor *Actors;
	Actor *MainParent;
	uint32 MainParentIndex;
	SceneManager *Owner;
	std::map<char*, uint32> ActiveActorMap;

	void InitActorSystem(uint32 count);

	void SetMainParent(char *name);
	void AddActor(char *name, Transform transform, char *tag = "");

	Actor* GetActor(char *name);

	uint32 GetActorIndex(char *name);

	void AttachChild(char *parentName, char *childName);
	void UpdateTransforms();

	void UpdateTransformChildren(Actor *transform, bool parentChanged);

	void UpdateOldTransforms(Actor *transform);

	Transform* GetTransform(char *name);
	void DestroyActor(char *name);
	void RemoveChildren(char *name, uint32 index);
};

struct SceneManager
{
	SceneManager(){}
	SceneManager(uint32 count)
	{
		InitScene(count);
	}

	ActorSystem ActorManager;
	RendererSystem RendererManager;
	CollisionSystem CollisionManager;
	AnimationSystem AnimationManager;

	Camera MainCamera;
	vec2f MousePos;
	float Delta;
	bool Debug;

	void InitScene(uint32 count);

	void UpdateScene(float delta);

	void RenderScene(bool debug);
};
