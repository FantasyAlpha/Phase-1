#pragma once

#include <ResourceManager.h>
#include <HoverSwitch.h>


struct MainMenu
{
public:
	MainMenu(SceneManager *scene, Game_Resources *resources, char *menuRoot);

	void InitMainMenu();

public:
	SceneManager *Scene;

	Game_Resources *Resources;

	char *MenuSubRoot;
};