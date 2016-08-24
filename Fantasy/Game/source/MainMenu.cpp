#include <MainMenu.h>

MainMenu::MainMenu(SceneManager *scene, Game_Resources *resources, char *menuRoot)
{
	Scene = scene;
	Resources = resources;
	MenuSubRoot = menuRoot;
}

void MainMenu::InitMainMenu()
{
	AddTexture(Resources, LoadTexture("resources\\textures\\Summoner's Tale 2.png"), "back");
	AddTexture(Resources, LoadTexture("resources\\textures\\continue.png"), "continue");
	AddTexture(Resources, LoadTexture("resources\\textures\\exit.png"), "exit");
	GameObject *currentRoot = Scene->CurrentRoot;

	Scene->EnableSubRoot(MenuSubRoot);

	GameObject *background = Scene->AddObject("background");
	GameObject *newGame = Scene->AddObject("new game");
	GameObject *exitGame = Scene->AddObject("exit game");
	/*
	*/
	background->AddComponent(&SpriteRenderer("bg0"
		, Sprite{ vec3f(), vec2f(2000, 1200) 
		, Material{ GetTexture(Resources, "Empty"), vec4f(0, 0.1f, 0.3f, 1) } }));
	
	background->AddComponent(&SpriteRenderer("bg"
		, Sprite{ vec3f(80.0f, -15.0f), vec2f(1280, 720)
		, Material{ GetTexture(Resources, "back"), vec4f(1, 1, 1, 1) } }));

	newGame->AddComponent(&SpriteRenderer("new"
		, Sprite{ vec3f(), vec2f(200, 80)
		, Material{ GetTexture(Resources, "continue"), vec4f(1, 1, 1, 1) } }));

	exitGame->AddComponent(&SpriteRenderer("exit"
		, Sprite{ vec3f(0, -120.0f), vec2f(200, 80)
		, Material{ GetTexture(Resources, "exit"), vec4f(1, 1, 1, 1) } }));

	Scene->CurrentRoot->AddComponent(&SwitchState("switch1", "State 1"));
	Scene->CurrentRoot->AddComponent(&SwitchState("switch2", "Quit Game"));
	Scene->CurrentRoot->AddComponent(&Hover("hover"));

	Hover *hover = (Hover *)Scene->CurrentRoot->GetComponent("hover", "HOVER");

	hover->AddButton((SpriteRenderer *)newGame->GetComponent("new", "RENDERABLE"));
	hover->AddButton((SpriteRenderer *)exitGame->GetComponent("exit", "RENDERABLE"));

	Scene->CurrentRoot->AddComponent(&HoverSwitch("hs1"
		, (SwitchState *)Scene->CurrentRoot->GetComponent("switch1", "SWITCH"), hover, "new"));

	Scene->CurrentRoot->AddComponent(&HoverSwitch("hs2"
		, (SwitchState *)Scene->CurrentRoot->GetComponent("switch2", "SWITCH"), hover, "exit"));

	if (currentRoot)
	{
		Scene->EnableSubRoot(currentRoot->Name);
	}
}