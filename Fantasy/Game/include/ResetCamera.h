#pragma once

#include <GameComponent.h>
#include <SceneManager.h>

struct ResetCamera : public GameComponent
{
	ResetCamera(vec2f size)
	{
		Name = "Reset";
		Type = "RESET CAM SIZE";
		Size = sizeof(ResetCamera);
		CamSize = size;
		ResetDone = true;
	}

	virtual void Update(float delta, Game_Input *input)
	{
		Scene->MainCamera.Size = CamSize;

		if (Scene->CurrentRoot->Name != "State 1")
		{
			Scene->MainCamera.Eye = vec3f();
		}
		else
		{
			if (!ResetDone)
			{
				Scene->MainCamera.Eye.X = Scene->GetGameObject("player")->ObjectTransform.Position.X;
				ResetDone = true;
			}
		}
	}

	vec2f CamSize;

	bool ResetDone;
};