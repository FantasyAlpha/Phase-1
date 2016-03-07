#pragma once

#include <map>
#include <Texture.h>
#include <Shader.h>
//ResourceManager

struct Game_Resources
{
	std::map<char *, Texture*> TextureResources;

	std::map<char *, Shader*> ShaderResources;
};

void AddTexture(Game_Resources *resources, Texture *texture, char *name);

Texture *GetTexture(Game_Resources *resources, char *name);

void AddShader(Game_Resources *resources, Shader *shader, char *name);

Shader *GetShader(Game_Resources *resources, char *name);
