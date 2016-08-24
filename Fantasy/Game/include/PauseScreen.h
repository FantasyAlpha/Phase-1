#pragma once

#include <SceneManager.h>
#include <ResourceManager.h>
#include <HoverSwitch.h>

struct PauseScreen
{
public:
	PauseScreen(SceneManager *scene, Game_Resources *resources, char *pauseRoot);

	void InitPauseGame();

public:
	SceneManager *Scene;

	Game_Resources *Resources;

	char *PauseSubRoot;
};