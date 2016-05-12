#pragma once

#include <unordered_map>
#include <Texture.h>
#include <SDL2\SDL.h>
#include <SDL_mixer.h>
//ResourceManager

struct Game_Resources
{
	std::unordered_map<char *, Texture> TextureResources;
	std::unordered_map<char *, Mix_Chunk *> SoundResources;
	std::unordered_map<char *, Mix_Music *> BGMResources;
};

void AddTexture(Game_Resources *resources, Texture texture, char *name);

Texture GetTexture(Game_Resources *resources, char *name);

void AddSound(Game_Resources *resources, char *path, char *name);

Mix_Chunk* GetSound(Game_Resources *resources, char *name);

void AddBGM(Game_Resources *resources, char *path, char *name);

Mix_Music* GetBGM(Game_Resources *resources, char *name);

void Play_Sound(Mix_Chunk *sound, int loop);

void Stop_Music();

void Resume_Music();

void Pause_Music();

void Play_Music(Mix_Music *bgm, int loop);