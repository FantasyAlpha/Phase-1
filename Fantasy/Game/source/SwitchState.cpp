#include <SwitchState.h>
#include <PlayBGM.h>
#include <ResetCamera.h>

SwitchState::SwitchState(char *name, char *nextState)
{
	Name = name;
	Type = "SWITCH";
	Size = sizeof(SwitchState);
	NextState = nextState;
	Switch = false;
}

void SwitchState::Update(float delta, Game_Input *input)
{
	if (Switch)
	{
		if (NextState == "Quit Game")
		{
			input->Exit = true;
		}
		else
		{
			Switch = false;
			PlayBGM *bgm = (PlayBGM *)Scene->CurrentRoot->GetComponent("play", "BGM");
			ResetCamera *rs = (ResetCamera *)Scene->CurrentRoot->GetComponent("Reset", "RESET CAM SIZE");
			if (bgm)
			{
				bgm->Playing = false;
			}

			if (rs)
			{
				rs->ResetDone = false;
			}

			Scene->EnableSubRoot(NextState);
		}
	}
}