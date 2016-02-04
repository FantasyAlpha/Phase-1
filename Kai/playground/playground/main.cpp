/*=======================================================================================================================
NOTE(kai): This file can:
1) Intialize the engine
2) run the engine
=======================================================================================================================*/

#include "main.h"

#undef main //To be able to use SDL because it defines it's own main

//
FILETIME GetLastWriteTime(char *path)
{
	FILETIME result = {};

	WIN32_FILE_ATTRIBUTE_DATA data;

	if (GetFileAttributesEx(path, GetFileExInfoStandard, &data))
	{
		result = data.ftLastWriteTime;
	}

	return result;
}

Game_Code LoadGameCode(char *path, char *temp)
{
	Game_Code result = {};

	result.LastWriteTimeDLL = GetLastWriteTime(path);

	CopyFile(path, temp, FALSE);

	result.GameCodeDLL = LoadLibraryA(temp);

	if (result.GameCodeDLL)
	{
		result.Game_Init = (game_init *)GetProcAddress(result.GameCodeDLL, "Game_Init");
		result.Game_Update = (game_update *)GetProcAddress(result.GameCodeDLL, "Game_Update");
		result.Game_Render = (game_render *)GetProcAddress(result.GameCodeDLL, "Game_Render");
	}

	return result;
}

void UnloadGameCode(Game_Code *gameCode)
{
	if (gameCode->GameCodeDLL)
	{
		FreeLibrary(gameCode->GameCodeDLL);
		gameCode->GameCodeDLL = NULL;
	}

	gameCode->Game_Init = NULL;	
	gameCode->Game_Update = NULL;
	gameCode->Game_Render = NULL;
}

void CatStrings(char *str1, int str1Size, char *str2, int str2Size, char *dest, int destSize)
{
	for (size_t i = 0; i < str1Size; i++)
	{
		*dest++ = *str1++;
	}

	for (size_t i = 0; i < str2Size; i++)
	{
		*dest++ = *str2++;
	}

	*dest++ = 0;
}

int StringLength(char *str)
{
	int count = 0;

	while (*str++)
	{
		count++;
	}

	return count;
}

void LoadFileDirectory(State *state)
{
	DWORD pathSize = GetModuleFileNameA(NULL, state->DLLFilePath, sizeof(state->DLLFilePath));
	state->OnePastLastSlash = state->DLLFilePath;

	for (char *scan = state->DLLFilePath; *scan; scan++)
	{
		if (*scan == '\\')
		{
			state->OnePastLastSlash = scan + 1;
		}
	}
}

void BuildFileFullPath(State *state, char *fileName, char *dest, int destSize)
{
	CatStrings(state->DLLFilePath, state->OnePastLastSlash - state->DLLFilePath, fileName, StringLength(fileName), dest, destSize);
}

// Initialize the engine and all systems associated with it(if there is any)
void InitSystem(char *title, int width, int height)
{
	state = {};

	LoadFileDirectory(&state);
	
	char DLLFullPath[MAX_PATH];
	BuildFileFullPath(&state, "playground game.dll", DLLFullPath, sizeof(DLLFullPath));

	char tempDLLFullPath[MAX_PATH];
	BuildFileFullPath(&state, "playground game_temp.dll", tempDLLFullPath, sizeof(tempDLLFullPath));

	char PDBFullPath[MAX_PATH];
	BuildFileFullPath(&state, "playground game.pdb", PDBFullPath, sizeof(PDBFullPath));

	char tempPDBFullPath[MAX_PATH];
	BuildFileFullPath(&state, "playground game_temp.pdb", tempPDBFullPath, sizeof(tempPDBFullPath));

	Window = WindowManager();
	Window.InitWindow(title, width, height);
	InputManager::InitInputManager(&Window);
	Game = {};

	Game = LoadGameCode(DLLFullPath, tempDLLFullPath);
	CopyFile(PDBFullPath, tempPDBFullPath, FALSE);

	Game.Game_Init();
}

//Begin running the engine
void Run()
{
	Running = true;
	MainLoop();
}

//stop the engine
void Stop()
{
	Running = false;
}

//Release resources (if there is any) and destory  the window
void Release()
{
	//Destroy the window
	Window.DestroyWindow();
}

//Our main loop which should continue running as long as we don't quite the game
void MainLoop()
{
	char DLLFilePath[MAX_PATH];
	char *onePastLastSlash;

	DWORD pathSize = GetModuleFileNameA(NULL, DLLFilePath, sizeof(DLLFilePath));
	onePastLastSlash = DLLFilePath;

	for (char *scan = DLLFilePath; *scan; scan++)
	{
		if (*scan == '\\')
		{
			onePastLastSlash = scan + 1;
		}
	}
	char DLLFullPath[MAX_PATH];
	BuildFileFullPath(&state, "playground game.dll", DLLFullPath, sizeof(DLLFullPath));

	char tempDLLFullPath[MAX_PATH];
	BuildFileFullPath(&state, "playground game_temp.dll", tempDLLFullPath, sizeof(tempDLLFullPath));

	char PDBFullPath[MAX_PATH];
	BuildFileFullPath(&state, "playground game.pdb", PDBFullPath, sizeof(PDBFullPath));

	char tempPDBFullPath[MAX_PATH];
	BuildFileFullPath(&state, "playground game_temp.pdb", tempPDBFullPath, sizeof(tempPDBFullPath));

	while (Running)
	{
		//Update everything
		Update();

		if (Window.IsCloseRequested())
		{
			Stop();
		}

		FILETIME newWriteTimeDLL = GetLastWriteTime(DLLFullPath);
		FILETIME newWriteTimePDB = GetLastWriteTime(PDBFullPath);
		if (CompareFileTime(&newWriteTimeDLL, &Game.LastWriteTimeDLL) != 0 && CompareFileTime(&newWriteTimeDLL, &Game.LastWriteTimePDB) != 0)
		{
			UnloadGameCode(&Game);
			CopyFile(PDBFullPath, tempPDBFullPath, FALSE);
			Game = LoadGameCode(DLLFullPath, tempDLLFullPath);
			Game.Game_Init();
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
void Render()
{
	//Clear the window
	Window.Clear();

	//Render the game
	Game.Game_Render();

	//Render the window
	Window.RenderWindow();
}

//Update all of the subsystems in the engine
void Update()
{
	//Update the window
	Window.UpdateWindow();

	//Update the game
	Game.Game_Update();
}

int main()
{
	//Instance of the engine's core 
	InitSystem("Kai engine", 1280, 720);
	
	//Start the engine
	Run();

	return 0;
}