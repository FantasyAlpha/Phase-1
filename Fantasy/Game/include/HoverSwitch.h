#pragma once

#include <vector>
#include <Hover.h>

struct HoverSwitch
	: public GameComponent
{
	HoverSwitch(char *name, SwitchState *switchState, Hover *hover, char *buttonName);

	virtual void Update(float delta, Game_Input *input);

	SwitchState *Switch;
	Hover *HoverList;
	char *ButtonName;
};
