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
	CollisionSystem CollisionManager;
	AnimationSystem AnimationManager;

	Camera MainCamera;
	vec2f MousePos;
	float Delta;
	bool Debug;

	void InitWorld(StackAllocator *sourceAllocator);	

	void UpdateWorld(float delta);

	void RenderWorld(bool debug);
};

