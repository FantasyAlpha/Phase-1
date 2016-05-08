/*=======================================================================================================================
NOTE(kai): This file can:
1) Intialize the engine
2) run the engine
=======================================================================================================================*/

#include "main.h"
#include <io.h>
#include <stdio.h>
#include <fcntl.h>
#include <iostream>

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
		result.Game_Shutdown = (game_shutdown *)GetProcAddress(result.GameCodeDLL, "Game_Shutdown");
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
	gameCode->Game_Shutdown = NULL;
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
	
	Dimensions.Width = (float)Window.Width;
	Dimensions.Height = (float)Window.Height;

	Game = LoadGameCode(DLLFullPath, tempDLLFullPath);
	CopyFile(PDBFullPath, tempPDBFullPath, FALSE);
	Game.Game_Init(Dimensions);
}

void ProcessInput(Game_Input *input)
{
	for (size_t i = 0; i < BUTTON_COUNT; i++)
	{
		if (i < BUTTON_COUNT - 3)
		{
			input->Buttons[i].KeyDown = IsKeyDown(&Keys, input->Buttons[i].Button);
			input->Buttons[i].KeyUp = IsKeyDown(&Keys, input->Buttons[i].Button);
		}
		else
		{
			input->Buttons[i].KeyDown = Keys.ButtonsDown[i - (BUTTON_COUNT - 3)];
			input->Buttons[i].KeyUp = Keys.ButtonsUp[i - (BUTTON_COUNT - 3)];
		}
	}

	for (size_t i = 0; i < GUI_BUTTON_COUNT; i++)
	{
		if (i < GUI_BUTTON_COUNT - 3)
		{
			GUIInput.Buttons[i].KeyDown = IsKeyDown(&Keys, GUIInput.Buttons[i].Button);
			GUIInput.Buttons[i].KeyUp = IsKeyDown(&Keys, GUIInput.Buttons[i].Button);
		}
		else
		{
			GUIInput.Buttons[i].KeyDown = Keys.ButtonsDown[i - (GUI_BUTTON_COUNT - 3)];
			GUIInput.Buttons[i].KeyUp = Keys.ButtonsUp[i - (GUI_BUTTON_COUNT - 3)];
		}
	}

	GUIInput.Key.Button = Keys.Key;
	GUIInput.Key.KeyUp = IsKeyUp(&Keys, Keys.Key);
	GUIInput.Char = Keys.Char;
	GUIInput.MouseX = Keys.MouseX;
	GUIInput.MouseY = Keys.MouseY;	

	GUI_ProcessInput(&GUIInput);

	input->MousePos = vec2f(Keys.MouseX, Keys.MouseY);

	Keys.ButtonsUp[0] = false;
	Keys.ButtonsUp[1] = false;
	Keys.ButtonsUp[2] = false;

	//Keys.Key = 1000000000;
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
	Game.Game_Shutdown();
	timeEndPeriod(1);
	wglMakeCurrent(NULL, NULL);
	ReleaseDC(Window.Window, GetDC(Window.Window));
	//Destroy the window
	DestroyWindow(Window.Window);
}

LARGE_INTEGER GetTicks()
{
	LARGE_INTEGER result;
	
	QueryPerformanceCounter(&result);

	return result;
}

/*
	1 second  ===> X ticks
	n second  ===> Y ticks

	wanted ==> n seconds

	given ==> X = PerformanceFrequency
	given ==> Y = start - end ==> (start, end = GetWallClock)

*/
float GetSecondsElapsed(LARGE_INTEGER start, LARGE_INTEGER end)
{
	float result = ((float)(end.QuadPart - start.QuadPart)) / (float)TicksPerSecond;

	return result;
}

void PrintTimeElapsed(LARGE_INTEGER start, LARGE_INTEGER end)
{
	float msPerFrame = 1000.0f * GetSecondsElapsed(start, end);

	float fps = 1000.0f / msPerFrame;

	std::cout<< "ms/f: " << msPerFrame << ", FPS: " << fps << "\n";
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

	//Input = {};
	GUIInput = {};

	{
		Input.UP.Button = VK_UP;
		Input.DOWN.Button = VK_DOWN;
		Input.RIGHT.Button = VK_RIGHT;
		Input.LEFT.Button = VK_LEFT;

		Input.W.Button = 'W';
		Input.S.Button = 'S';
		Input.A.Button = 'A';
		Input.D.Button = 'D';
		Input.P.Button = 'P';
		Input.P.Button = 'O';

		Input.MOUSE_LEFT.Button = MK_LBUTTON;
		Input.MOUSE_MIDDLE.Button = MK_MBUTTON;
		Input.MOUSE_RIGHT.Button = MK_RBUTTON;
	}

	{
		GUIInput.UP.Button = VK_UP;
		GUIInput.DOWN.Button = VK_DOWN;
		GUIInput.RIGHT.Button = VK_RIGHT;
		GUIInput.LEFT.Button = VK_LEFT;
		GUIInput.BACKSPACE.Button = VK_BACK;
		
		GUIInput.MOUSE_LEFT.Button = MK_LBUTTON;
		GUIInput.MOUSE_RIGHT.Button = MK_RBUTTON;
		GUIInput.MOUSE_MIDDLE.Button = MK_MBUTTON;
	}
	GUI_Init(Window.Width, Window.Height, &GUIInput);
	LARGE_INTEGER performanceFrequency;
	QueryPerformanceFrequency(&performanceFrequency);
	TicksPerSecond = performanceFrequency.QuadPart;

	int monitorRefreshHZ = 60;
	HDC deviceContext = GetDC(Window.Window);
	int refreshHz = GetDeviceCaps(deviceContext, VREFRESH);
	ReleaseDC(Window.Window, deviceContext);

	if (refreshHz > 1)
	{
		monitorRefreshHZ = refreshHz;
	}

	float gameUpdateHZ = (float)(monitorRefreshHZ);
	float targetSecondsPerFrame = 1.0f / gameUpdateHZ;

	UINT desiredSchedulerTime = 1;
	bool sleepIsSmaller = true;//timeBeginPeriod(desiredSchedulerTime) == TIMERR_NOERROR;

	LARGE_INTEGER lastTick = GetTicks();
	float updateTime = 0;
	int updates = 0;
	double frames = 0;
	double frameTime = 0;

	bool createWindow = false;
	ImVec4 clearColor = ImVec4(0, 0.3f, 0.4f, 1.0f);
	ClearColor color = ClearColor{ 0, 0.3f, 0.4f, 1.0f };

	start_loop = clock();

	while (IsRunning)
	{

		end_loop = clock();
		delta = ((end_loop - start_loop) / double(CLOCKS_PER_SEC));
		start_loop = end_loop;

		std::cout << "delta is  " << delta << std::endl;

		FILETIME newWriteTimeDLL = GetLastWriteTime(DLLFullPath);
		FILETIME newWriteTimePDB = GetLastWriteTime(PDBFullPath);
		
		if (CompareFileTime(&newWriteTimeDLL, &Game.LastWriteTimeDLL) != 0 && CompareFileTime(&newWriteTimeDLL, &Game.LastWriteTimePDB) != 0)
		{
			UnloadGameCode(&Game);
			CopyFile(PDBFullPath, tempPDBFullPath, FALSE);
			Game = LoadGameCode(DLLFullPath, tempDLLFullPath);
			Game.Game_Init(Dimensions);
		}

		LARGE_INTEGER gameTimerStart = GetTicks();
		ProcessPendingMessages(&Keys);

		ProcessInput(&Input);
		//Clear the window
		ClearWindow();
		GUI_NewFrame();
		{
			if (ImGui::Button("KAI"))
			{
				createWindow ^= 1;
			}
			ImGui::ColorEdit3("CLEAR COLOR!", (float*)&clearColor);
			ImGui::SetNextWindowSize(ImVec2(100, 300), ImGuiSetCond_FirstUseEver);
			//ImGui::Begin("KAAI", &createWindow);
			ImGui::Text("KAAAAAAAAAAAAAAAAAAI");
			//ImGui::End();
		}
		//Update the game
		Game.Game_Update(&Input, delta);

		//Render the game
		Game.Game_Render();
		color.r = clearColor.x;
		color.g = clearColor.y;
		color.b = clearColor.z;
		color.a = clearColor.w;
		SetClearColor(color);
		ImGui::Render();
		
		LARGE_INTEGER gameTimerEnd = GetTicks();
		frameTime += (double)(1000.0f * GetSecondsElapsed(gameTimerStart, gameTimerEnd));
		frames++;
		//frames += 1000.0f / (double)(1000.0f * GetSecondsElapsed(gameTimerStart, gameTimerEnd));
		//PrintTimeElapsed(lastTick, gameTimerEnd);

		float secondsElapsedForFrame = GetSecondsElapsed(lastTick, GetTicks());

		if (secondsElapsedForFrame < targetSecondsPerFrame)
		{
			if (sleepIsSmaller)
			{
				DWORD sleepTime = (DWORD)(1000.0f * (targetSecondsPerFrame - secondsElapsedForFrame));

				if (sleepTime > 0)
				{
					Sleep(sleepTime);
				}
			}

			while (secondsElapsedForFrame < targetSecondsPerFrame)
			{
				secondsElapsedForFrame = GetSecondsElapsed(lastTick, GetTicks());
			}
			updates++;
		}

		updateTime += GetSecondsElapsed(lastTick, GetTicks());

		if (updateTime >= 1.0f)
		{
			double avgFPS = 1000.0f / ((frameTime) / frames);
			std::cout << "UPS: " << updates << ", average FPS: " << avgFPS << ", average work/frame: " << (frameTime) / frames << "\n";
			
			frames = 0;
			frameTime = 0;
			updates = 0;
			updateTime = 0;
		}
		
		LARGE_INTEGER endTick = GetTicks();
		//PrintTimeElapsed(lastTick, endTick);
		lastTick = endTick;
		
		//Render the window
		RenderWindow(Window.Window);

		/*
		//calc fps 
		calcfps();
		static int framecount = 0;
		framecount++;
		if (framecount == 10) {
			framecount = 0;
			std::cout << "frame per second is : " << (fps) << std::endl;

		}
		//QueryPerformanceCounter(&t_current_loop);
		end_loop = clock();

		//float frameticks = (t_current_loop.QuadPart - t_previous_loop.QuadPart) / ((frequency_loop.QuadPart) / 1000.0);

		float frameticks = ((float)(end_loop - start_loop) / CLOCKS_PER_SEC) * 1000.0f;

		//print the current fps 

		// std::cout << 1000/frameticks << std::endl;

		if (1000.0f / max_fps > frameticks){

			Sleep(1000.0f / max_fps - frameticks);
		}
		*/

	}

	//Release resources (if there is any) and destory  the window
	Release();

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

	static const int num_samples = 10;
	static float frametimes[num_samples];
	static int currentframe = 0;

	//QueryPerformanceFrequency(&frequency);


	//QueryPerformanceCounter(&t_previous);
	static clock_t start = clock();
	//QueryPerformanceCounter(&t_current);	
	end = clock();
	//frametime = (t_current.QuadPart - t_previous.QuadPart) / ((frequency.QuadPart) / 1000.0);

	frametime = ((double)(end - start) / CLOCKS_PER_SEC) * 1000.0f;

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


	float frametimeaveraged = 0;
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

	return 0;
}