#include "Entity.h"

BoundingBox2D CreateBoundingBox2D(vec3 position, vec2 size)
{
	BoundingBox2D boundingBox; 
	
	boundingBox.Size = size;
	boundingBox.Position = position; 
	
	return boundingBox;
}

void AddBoundingBox2D(Entity *entity, vec2 bounds)
{
	entity->Collider2D = CreateBoundingBox2D(entity->Local_Transform.Position, bounds);
}

void AddEntitiesToLayer(Layer *layer, Entity *entities, int entityCount)
{
	layer->Entities = entities;
	layer->EntityCount = entityCount;
}

void AddLayers(World *world, Layer *layers, int layerCount)
{
	for (int i = 0; i < layerCount; i++)
	{
		layers[i].Collider2D = CreateBoundingBox2D(layers[i].Local_Transform.Position, layers[i].LayerSize);
	}

	world->Layers = layers;

	world->LayerCount = layerCount;
}

void RenderEntities(World *world)
{
	for (int i = 0; i < world->LayerCount; i++)
	{
		for (int j = 0; j < (world->Layers)[i].EntityCount; j++)
		{
			DrawMesh(&(world->Layers)[i].Entities[j].Renderable);
		}
	}
}
