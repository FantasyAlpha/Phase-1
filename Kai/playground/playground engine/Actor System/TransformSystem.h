#pragma once 

#include "Components.h"
#include "ActorSystem.h"

void InitTransformSystem(StackAllocator *sourceAllocator, World *manager);

void AddTransform(World *manager, char *actorName, TransformComponent transform);

uint32 GetTransformIndex(World *manager, char *name);

char* GetTransformOwner(World *manager, uint32 index);

void RemoveTransformChildren(World *manager, char *name, uint32 index);

void RemoveTransform(World *manager, char *name);

void AddTransformChild(World *manager, char *parentActorName, char *childActorName);

void SetMainParent(World *manager, char *name);

void UpdateTransformSystem(World *manager);

void UpdateTransformChildren(World *manager, TransformComponent *transform, bool parentChanged);

void UpdateOldTransform(World *manager, TransformComponent *transform);

Transform* GetTransform(World *manager, char *name);