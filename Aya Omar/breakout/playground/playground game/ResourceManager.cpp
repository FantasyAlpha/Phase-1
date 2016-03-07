#include "ResourceManager.h"

void AddTexture(Game_Resources *resources, Texture *texture, char *name)
{
	
	resources->TextureResources[name] = texture;
}

Texture *GetTexture(Game_Resources *resources, char *name)
{
	return resources->TextureResources.at(name);
}

void AddShader(Game_Resources *resources, Shader *shader, char *name)
{
	resources->ShaderResources[name] = shader;
}

Shader *GetShader(Game_Resources *resources, char *name)
{
	return resources->ShaderResources.at(name);
}
