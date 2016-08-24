#pragma once

#include <SceneManager.h>
#include <ResourceManager.h>
struct PlayBGM : public GameComponent
{
	PlayBGM(Game_Resources *resources)
	{
		Name = "play";
		Type = "BGM";
		Size = sizeof(PlayBGM);
		Resources = resources;
		Playing = false;
	}

	virtual void Update(float delta, Game_Input *input)
	{
		if (Scene->CurrentRoot)
		{
			if (Scene->CurrentRoot->Name == "State 2")
			{
				if (!Playing)
				{
					Play_Music(Resources, "menu", -1);
					Playing = true;
				}
			}
			else if (Scene->CurrentRoot->Name == "State 1")
			{
				if (!Playing)
				{
					Play_Music(Resources, "game", -1);
					Playing = true;
				}
			}
		}
	}

	Game_Resources *Resources;
	bool Playing;
};