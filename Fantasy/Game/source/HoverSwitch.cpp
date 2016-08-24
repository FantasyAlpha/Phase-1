#include <HoverSwitch.h>

HoverSwitch::HoverSwitch(char *name, SwitchState *switchState, Hover *hover, char *buttonName)
{
	Name = name;
	HoverList = hover;
	Switch = switchState;
	ButtonName = buttonName;
	Size = sizeof(HoverSwitch);
	Type = "HOVER SWITCH";
}

void HoverSwitch::Update(float delta, Game_Input *input)
{
	if (HoverList->GetCurrentButton() == ButtonName)
	{
		if (input->Enter.KeyUp)
		{
			Switch->Switch = true;
			HoverList->Reset();
		}
	}
}