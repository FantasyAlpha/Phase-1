#pragma once

#include <SwitchState.h>

struct PauseGame : public GameComponent
{
	PauseGame(SwitchState *state);

	virtual void Update(float delta, Game_Input *input);

	SwitchState *Switch;
};