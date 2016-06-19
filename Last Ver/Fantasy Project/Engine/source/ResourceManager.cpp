#include <ResourceManager.h>

void AddTexture(Game_Resources *resources, Texture texture, char *name)
{	
	resources->TextureResources[name] = texture;
}

Texture GetTexture(Game_Resources *resources, char *name)
{
	return resources->TextureResources.at(name);
}

void AddSound(Game_Resources *resources, char *path, char *name)
{
	//Load sound effects using Mix_LoadWAV.
	//This can load WAVE, AIFF, RIFF, OGG, and VOC files.
	Mix_Chunk *sound = Mix_LoadWAV(path);
	if (sound == NULL)
	{
		printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
	}
	else
	{
		resources->SoundResources[name] = sound;
	}
}

Mix_Chunk* GetSound(Game_Resources *resources, char *name)
{
	return resources->SoundResources.at(name);
}

void AddBGM(Game_Resources *resources, char *path, char *name)
{
	// Load music using "Mix_LoadMUS".
	//This can load WAVE, MOD, MIDI, OGG, MP3, FLAC, and any file that you use a command to play with

	//gMusic = Mix_LoadMUS( "./res/audio/beat.wav" );
	//gMusic = Mix_LoadMUS( "./res/audio/musafer_m3_elquran.mp3" );
	Mix_Music *bgm = Mix_LoadMUS(path);

	if (bgm == NULL)
	{
		printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
	}
	else
	{
		resources->BGMResources[name] = bgm;
	}
}

Mix_Music* GetBGM(Game_Resources *resources, char *name)
{
	return resources->BGMResources.at(name);
}

void Play_Sound(Game_Resources *resources, char *name, int loop)
{
	Mix_PlayChannel(-1, GetSound(resources, name), loop);
}

void Stop_Music()
{
	Mix_HaltMusic();
}

void Resume_Music()
{
	Mix_ResumeMusic();
}

void Pause_Music()
{
	Mix_PauseMusic();
}

void Play_Music(Game_Resources *resources, char *name, int loop)
{
	Mix_PlayMusic(GetBGM(resources, name), loop);
}