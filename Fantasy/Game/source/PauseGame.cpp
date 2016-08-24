#include <PauseGame.h>

PauseGame::PauseGame(SwitchState *state)
{
	Name = "Pause";
	Type = "PAUSE GAME";
	Size = sizeof(PauseGame);
	Switch = state;
}

void PauseGame::Update(float delta, Game_Input *input)
{
	if (input->Escape.KeyUp)
	{
		Switch->Switch = true;
	}
}