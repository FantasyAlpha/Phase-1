/*=======================================================================================================================
NOTE(kai): This file can:
1) Intialize the engine
2) run the engine
=======================================================================================================================*/

#include "main.h"
#include <io.h>
#include <stdio.h>
#include <fcntl.h>

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
	for (int i = 0; i < str1Size; i++)
	{
		*dest++ = *str1++;
	}

	for (int i = 0; i < str2Size; i++)
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

LRESULT CALLBACK WindowCallBack(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;

	switch (message)
	{
	case WM_DESTROY:
	{
		IsRunning = false;

		OutputDebugString("HERE\n");
	}
	break;

	case WM_QUIT:
	{
		IsRunning = false;

		OutputDebugString("HERE\n");
	}
	break;

	default:
	{
		result = DefWindowProc(window, message, wParam, lParam);
	}
	break;
	}


	return result;
}

// Initialize the engine and all systems associated with it(if there is any)
void InitSystem(HINSTANCE hInstance, char *title, int width, int height)
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

	Window = {};
	Window.WindowCallback = WindowCallBack;
	InitWindow(hInstance, &Window, title, width, height);
	InitInputManager(&Keys);
	Game = {};

	Game = LoadGameCode(DLLFullPath, tempDLLFullPath);
	CopyFile(PDBFullPath, tempPDBFullPath, FALSE);
	Game.Game_Init();
}

void ProcessInput(Game_Input *input)
{
	input->UP.KeyDown = IsKeyDown(&Keys, input->UP.Button);
	input->UP.KeyUp = IsKeyUp(&Keys, input->UP.Button);

	input->DOWN.KeyDown = IsKeyDown(&Keys, input->DOWN.Button);
	input->DOWN.KeyUp = IsKeyUp(&Keys, input->DOWN.Button);

	input->RIGHT.KeyDown = IsKeyDown(&Keys, input->RIGHT.Button);
	input->RIGHT.KeyUp = IsKeyUp(&Keys, input->RIGHT.Button);

	input->LEFT.KeyDown = IsKeyDown(&Keys, input->LEFT.Button);
	input->LEFT.KeyUp = IsKeyUp(&Keys, input->LEFT.Button);
}

//Begin running the engine
void Run()
{
	IsRunning = true;
	MainLoop();
}

//stop the engine
void Stop()
{
	IsRunning = false;
}

//Release resources (if there is any) and destory  the window
void Release()
{
	wglMakeCurrent(NULL, NULL);
	ReleaseDC(Window.Window, GetDC(Window.Window));
	//Destroy the window
	DestroyWindow(Window.Window);
}

//Our main loop which should continue running as long as we don't quite the game
static void MainLoop()
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

	Input = {};
	Input.UP.Button = VK_UP;
	Input.DOWN.Button = VK_DOWN;
	Input.RIGHT.Button = VK_RIGHT;
	Input.LEFT.Button = VK_LEFT;

	while (IsRunning)
	{

		//QueryPerformanceFrequency(&frequency_loop);
		//QueryPerformanceCounter(&t_previous_loop);
		start_loop = clock();
		ProcessPendingMessages(&Keys);

		ProcessInput(&Input);
		//Update everything
		Update();

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
		if (IsKeyDown(&Keys, 'A'))
		{
			OutputDebugString("Key: a is pressed\n");
		}
		//Testing if A button is released
		if (IsKeyUp(&Keys, 'A'))
		{
			OutputDebugString("Key: a is released\n");
		}
		
		//Render everything
		Render();

		//calc fps 
    calcfps();
	static int framecount = 0;
	  framecount++;
	  if (framecount == 10) {
		  framecount = 0;
	 std::cout << "frame per second is : "<<(fps) << std::endl;

	  }
	  //QueryPerformanceCounter(&t_current_loop);
	  end_loop = clock();

	  //float frameticks = (t_current_loop.QuadPart - t_previous_loop.QuadPart) / ((frequency_loop.QuadPart) / 1000.0);

	    float frameticks = ((double)(end_loop - start_loop) / CLOCKS_PER_SEC) * 1000;

	  //print the current fps 

	  // std::cout << 1000/frameticks << std::endl;

	  if (1000 / max_fps > frameticks){
		  
	 Sleep(1000 / max_fps - frameticks);
	  }



	}

	//Release resources (if there is any) and destory  the window
	Release();

}

//Render all of the subsystems in the engine
void Render()
{
	//Clear the window
	ClearWindow();	
	//Render the game
	Game.Game_Render();
	//Render the window
	RenderWindow(Window.Window);
}

//Update all of the subsystems in the engine
void Update()
{
	//Update the game
	Game.Game_Update(&Input);
}

void CreateConsole()
{
	AllocConsole();

	HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);	
		
	int hCrt = _open_osfhandle((long)handle_out, _O_TEXT);
	FILE* hf_out = _fdopen(hCrt, "w");
	setvbuf(hf_out, NULL, _IONBF, 1);
	*stdout = *hf_out;

	HANDLE handle_in = GetStdHandle(STD_INPUT_HANDLE);
	hCrt = _open_osfhandle((long)handle_in, _O_TEXT);
	FILE* hf_in = _fdopen(hCrt, "r");
	setvbuf(hf_in, NULL, _IONBF, 128);
	*stdin = *hf_in;

}

//calculate frame per second 

void calcfps(){

	static const int num_samples=10;
	static float frametimes[num_samples];
	static int currentframe = 0;

	//QueryPerformanceFrequency(&frequency);


	//QueryPerformanceCounter(&t_previous);
	static clock_t start = clock();
   	//QueryPerformanceCounter(&t_current);	
   	end = clock();
	//frametime = (t_current.QuadPart - t_previous.QuadPart) / ((frequency.QuadPart) / 1000.0);

	frametime = ((double)(end - start) / CLOCKS_PER_SEC) * 1000;

	// make cycle from 0===> num_samples ====>0 by % module operator if num_samples = 3 then we go through 0 1 2 0 1 2 .............

	frametimes[currentframe % num_samples] = frametime;

	//t_previous.QuadPart = t_current.QuadPart;



	start = end;

	// how we will calc average : we make average for the current frame with the
	//previos ones so avergae firsttime frame 0/1 =0 then second 0+16/2 then  0+16+8/3 0+16+8+............../10 
	//count will be as follow 

	int count = 0;
	currentframe++;

	if (currentframe <num_samples){
		count = currentframe;
	}
	else {
		count = num_samples;
	}


	float frametimeaveraged=0;
	for (int i = 0; i < count; i++)
	{
		frametimeaveraged += frametimes[i];
	}
	frametimeaveraged /= count;

	// finally that is the value that we will use tp calc fps 

	// check it is >0 cause for first frame =0   fps in milliseconds 
	if (frametimeaveraged>0) {
		fps = 1000 / frametimeaveraged;
	}
	else {
		frametimeaveraged = 60;
	}

}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdShow)
{
	CreateConsole();
	//Instance of the engine's core 
	InitSystem(hInstance, "Kai engine", 1280, 720);
	
	//Start the engine
	Run();
	system("pause");
	return 0;
}