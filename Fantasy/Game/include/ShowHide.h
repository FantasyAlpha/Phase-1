#pragma once 

#include <SceneManager.h>

struct ShowHide : public GameComponent
{
	ShowHide(char *name, char *child, int key)
	{
		Name = name;
		Type = "SH";
		Size = sizeof(ShowHide);
		Child = child;
		Enable = key;
	}

	virtual void Init()
	{
		SetEnableKey(Enable);
	}

	virtual void Update(float delta, Game_Input *input)
	{
		if (input->Buttons[Enable].KeyUp)
		{
			Owner->GetChild(Child)->Enabled = !Owner->GetChild(Child)->Enabled;
		}
	}

	void SetEnableKey(int key)
	{
		Enable = Scene->GetInputKey(key);
	}

	int Enable;
	char *Child;
};