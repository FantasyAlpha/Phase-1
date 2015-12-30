#include "EngineCore.h"


EngineCore::EngineCore(char *title, int width, int height)
{
	InitEngine(title, width, height);
}


EngineCore::~EngineCore()
{
	if (game)
	{
		delete game;
	}

	if (Window)
	{
		delete Window;
	}
}

//Initialize the engine and all systems associated with it (if there is any)
void EngineCore::InitEngine(char *title, int width, int height)
{
	Window = new WindowManager();
	Window->InitWindow(title, width, height);
	InputManager::InitInputManager(Window);

	game = new Game();
	game->Init();
}

//Begin running the engine
void EngineCore::Run()
{
	Running = true;
	MainLoop();
}

//stop the engine
void EngineCore::Stop()
{
	Running = false;
}

//Release resources (if there is any) and destory  the window
void EngineCore::Release()
{
	//Destroy the window
	Window->DestroyWindow();
}

//Our main loop which should continue running as long as we don't quite the game
void EngineCore::MainLoop()
{
	while (Running)
	{
		//Update everything
		Update();
		
		if (Window->IsCloseRequested())
		{
			Stop();
		}

		/*NOTE(kai): TEST ONLY*/
		//Testing if A button is pressed
		if (InputManager::IsKeyDown(KEY::KEY_A))
		{
			std::cout << "Key: a is pressed\n";
		}
		//Testing if A button is released
		if (InputManager::IsKeyUp(KEY::KEY_A))
		{
			std::cout << "Key: a is released\n";
		}
		
		//Render everything
		Render();		
	}

	//Release resources (if there is any) and destory  the window
	Release();
}

//Render all of the subsystems in the engine
void EngineCore::Render()
{
	//Clear the window
	Window->Clear();

	//Render the game
	game->Render();

	//Render the window
	Window->RenderWindow();
}

//Update all of the subsystems in the engine
void EngineCore::Update()
{
	//Poll events using SDL
#if MODE_TYPE == SDL_MODE
	InputManager::Update(Window);
#endif

	//Update the window
	Window->UpdateWindow();

	//Update the game
	game->Update();
}