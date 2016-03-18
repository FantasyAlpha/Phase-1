#pragma once

#include "Components.h"
#include "ActorSystem.h"

void InitSpriteRendererSystem(StackAllocator *sourceAllocator, World *manager);

void AddRenderer(World *manager, char *actorName, SpriteRenderer renderer);

uint32 GetRendererIndex(World *manager, char *name);

void RemoveRenderer(World *manager, char *actorName);

void RendererSprites(World *manager);

void InitMainShader(World *manager, char *vertexShader, char *fragmentShader);
