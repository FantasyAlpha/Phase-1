#pragma once

#include <unordered_map>
#include "..\Memory\MemoryAllocator.h"
#include <vector>
#include "..\Graphics\Mesh.h"
#include "..\Graphics\Material.h"
#include "..\Graphics\Shader.h"
#include "..\General\Transform.h"

#define MAX_ACTOR_COUNT 100
#define MAX_SPRITE_COUNT 100
#define MAX_TRANSFORM_COUNT MAX_ACTOR_COUNT


struct ComponentData
{
	PoolAllocator Allocator;
	std::vector<uint32> ActiveComponents;
};

struct SpriteRenderer
{
	Mesh Sprite;
	Material SpriteMaterial;
};

#define SPRITE_RENDERER_SIZE sizeof(SpriteRenderer)
#define TOTAL_SPRITES_SIZE SPRITE_RENDERER_SIZE * MAX_SPRITE_COUNT

struct SpriteRendererSystem
{
	ComponentData Data;
	SpriteRenderer *Renderers;
	Shader MainShader;
};

struct TransformComponent
{
	Transform CurrentTransform;
	Transform OldTransform;
	mat4 ModelMatrix;
	char *ParentName;
	uint32 ParentIndex = MAX_TRANSFORM_COUNT + 1;
	std::vector<uint32> *Children = new std::vector<uint32>();
};
 
#define TRANSFORM_COMPONENT_SIZE sizeof(TransformComponent)
#define TOTAL_TRANSFORMS_SIZE TRANSFORM_COMPONENT_SIZE * MAX_TRANSFORM_COUNT

struct TransformSystem
{
	ComponentData Data;
	TransformComponent *Transforms;
	TransformComponent *MainParent;
};

