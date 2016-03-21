#pragma once

#include <unordered_map>
#include <Texture.h>
//ResourceManager

struct Game_Resources
{
	std::unordered_map<char *, Texture> TextureResources;
};

void AddTexture(Game_Resources *resources, Texture texture, char *name);

Texture GetTexture(Game_Resources *resources, char *name);
