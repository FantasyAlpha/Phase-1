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
	AnimationSystem AnimationManager;

	Camera MainCamera;
	vec2 MousePos;
	float Delta;
	bool Debug;

	void InitWorld(StackAllocator *sourceAllocator);	

	void UpdateWorld();

	void RenderWorld(bool debug);
};

