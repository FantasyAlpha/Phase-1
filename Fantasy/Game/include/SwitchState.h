#pragma once

#include <SceneManager.h>

struct SwitchState : public GameComponent
{
	SwitchState(char *name, char *nextState);

	virtual void Update(float delta, Game_Input *input);

	char *NextState;
	bool Switch;
};