#pragma once

#include "Mesh.h"
#include <vector>

struct BoundingBox2D
{
	vec3 Position;
	vec2 Size;
};

struct Entity
{
	Mesh Renderable;
	char *Type;
	BoundingBox2D Collider2D;
	Transform Local_Transform;
	Transform Global_Transform;
	Entity *Parent;
};

struct Layer
{
	Entity *Entities;
	BoundingBox2D Collider2D;
	Transform Local_Transform;
	Transform Global_Transform;
	vec2 LayerSize;
	int EntityCount;
	int DepthOrder;
};

struct World
{
	Layer *Layers;
	int LayerCount;
};

BoundingBox2D CreateBoundingBox2D(vec3 position, vec2 size);

void AddBoundingBox2D(Entity *entity, vec2 bounds);

void AddEntitiesToLayer(Layer *layer, Entity *entities, int entityCount);

void AddLayers(World *world, Layer *layers, int layerCount);

void RenderEntities(World *world);