#pragma once

#include <vector>
#include <unordered_map>
#include <MemoryAllocator.h>
#include <Components.h>

struct World
{
	ActorSystem ActorManager;
	RendererSystem RendererManager;
	TransformSystem TransformManager;
	Camera MainCamera;

	void InitWorld(StackAllocator *sourceAllocator);	

	void UpdateWorld();

	void RenderWorld();
};

