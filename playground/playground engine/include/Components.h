#pragma once

#include <unordered_map>
#include <vector>
#include <MemoryAllocator.h>
#include <Cinder\CinderMemory.h>
#include <Texture.h>
#include <Mesh.h>
#include <Shader.h>
#include <Transform.h>
#include <Types.h>

struct SceneManager;

struct Physics{
	float acceleration;
	float Gravity;
	float mass;

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
	Collider *Colliders;
	SceneManager *Owner;
	std::unordered_map<char*, uint32> Owner_ComponentMap;
	Physics physics;

	void InitCollisionSystem(uint32 count);
	void AddComponent(char *name, vec3f *pos , vec2f size ,bool ground, bool wall ,bool trigger);
	void RemoveComponent(char *name);
	void UpdateCollisionSystem(float delta);
	uint32 GetColliderIndex(char *name);
	Collider* GetCollider(char *name);
	bool CheckGroundCollision(char*name);

};


struct Renderable
{
	vec2f Size;

	Material RenderableMaterial;

	AnimationClip *Clip;

	uint32 OwnerIndex;

	bool WithClip;
};

struct RendererSystem
{
	Cinder::Memory::MemoryPool Pool;

	Renderable *Renderables;

	MeshBatch Renderer;
	MeshBatch DebugRenderer;

	Shader MainShader;
	Shader DebugShader;

	SceneManager *Owner;

	std::unordered_map<char*, uint32> Owner_ComponentMap;

	void InitRendererSystem(uint32 count);

	void AddComponent(char *actorName, vec2f size, Material material, AnimationClip *clip = 0);

	uint32 GetRenderableIndex(char *name);
	Renderable* GetRenderable(char *name);

	void RemoveRenderer(char *actorName);

	void RenderAllActive();
	void RenderDebugShapes();

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
	float Delta;

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

	std::unordered_map<char*, uint32> Name_ComponentMap;

	void InitAnimationSystem(uint32 count);

	void AddComponent(char *name, uint32 maxCountHorizontal, uint32 maxCountVertical, uint32 *indices
		, uint32 frameCount, float runSpeed_FPS, bool loop);

	uint32 GetAnimationClipIndex(char *name);
	AnimationClip* GetAnimationClip(char *name);

	void RemoveAnimationClip(char *name);

	void SwitchAnimation(char *actor, char *animation);
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
};

struct ActorSystem
{
	Cinder::Memory::MemoryPool Pool;
	Actor *Actors;
	Actor *MainParent;
	uint32 MainParentIndex;
	SceneManager *Owner;
	std::unordered_map<char*, uint32> ActiveActorMap;

	void InitActorSystem(uint32 count);

	void SetMainParent(char *name);
	void AddActor(char *name, Transform transform);

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
