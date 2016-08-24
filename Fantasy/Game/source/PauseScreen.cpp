#include <PauseScreen.h>

PauseScreen::PauseScreen(SceneManager *scene, Game_Resources *resources, char *pauseRoot)
{
	Scene = scene;
	Resources = resources;
	PauseSubRoot = pauseRoot;
}

void PauseScreen::InitPauseGame()
{
	AddTexture(Resources, LoadTexture("resources\\textures\\resume.png"), "resume");
	AddTexture(Resources, LoadTexture("resources\\textures\\return.png"), "return");

	GameObject *currentRoot = Scene->CurrentRoot;

	Scene->EnableSubRoot(PauseSubRoot);

	GameObject *background = Scene->AddObject("pause bg");
	GameObject *resume = Scene->AddObject("res");
	GameObject *returnToMenu = Scene->AddObject("ret");

	background->AddComponent(&SpriteRenderer("bg"
		, Sprite{ vec3f(80, -15), vec2f(1280, 720) 
		, Material{ GetTexture(Resources, "back"), vec4f(0.5f, 0.5f, 0.5f, 1) } }));

	resume->AddComponent(&SpriteRenderer("res"
		, Sprite{ vec3f(), vec2f(200, 80) 
		, Material{ GetTexture(Resources, "resume"), vec4f(1, 1, 0, 1) } }));

	returnToMenu->AddComponent(&SpriteRenderer("ret"
		, Sprite{ vec3f(0, -120), vec2f(200, 80)
		, Material{ GetTexture(Resources, "return"), vec4f(1, 1, 1, 1) } }));

	Scene->CurrentRoot->AddComponent(&SwitchState("switch1", "State 1"));
	Scene->CurrentRoot->AddComponent(&SwitchState("switch2", "State 2"));
	Scene->CurrentRoot->AddComponent(&Hover("hover"));

	Hover *hover = (Hover *)Scene->CurrentRoot->GetComponent("hover", "HOVER");

	hover->AddButton((SpriteRenderer *)resume->GetComponent("res", "RENDERABLE"));
	hover->AddButton((SpriteRenderer *)returnToMenu->GetComponent("ret", "RENDERABLE"));

	Scene->CurrentRoot->AddComponent(&HoverSwitch("hs1"
		, (SwitchState *)Scene->CurrentRoot->GetComponent("switch1", "SWITCH"), hover, "res"));

	Scene->CurrentRoot->AddComponent(&HoverSwitch("hs2"
		, (SwitchState *)Scene->CurrentRoot->GetComponent("switch2", "SWITCH"), hover, "ret"));

	if (currentRoot)
	{
		Scene->EnableSubRoot(currentRoot->Name);
	}
}