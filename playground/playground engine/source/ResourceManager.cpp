#include <ResourceManager.h>

void AddTexture(Game_Resources *resources, Texture texture, char *name)
{	
	resources->TextureResources[name] = texture;
}

Texture GetTexture(Game_Resources *resources, char *name)
{
	return resources->TextureResources.at(name);
}
