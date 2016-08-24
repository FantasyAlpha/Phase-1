#pragma once

#include <vector>
#include <SwitchState.h>
#include <SpriteRenderer.h>

struct Hover : public GameComponent
{
public:
	Hover(char *name);

	virtual void Destroy();

	virtual void Update(float delta, Game_Input *input);

	void AddButton(SpriteRenderer *renderer);

	char* GetCurrentButton();

	void Reset();

	std::vector<SpriteRenderer *> *ButtonRenderers;

	uint32 CurrentRenderer;
};