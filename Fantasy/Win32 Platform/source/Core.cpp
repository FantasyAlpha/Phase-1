#include <Core.h>
#include <io.h>
#include <stdio.h>
#include <fcntl.h>
#include <iostream>
#include <SDL_mixer.h>

bool Win32_Core::IsRunning = false; 
Win32_Core *Win32_Core::Core = NULL;

Win32_Core* Win32_Core::CreateCore()
{
	if (!Core)
	{
		Core = new Win32_Core;
	}

	return Core;
}

void Win32_Core::SwitchFullScreen(HWND hwnd)
{
	DWORD dwStyle = GetWindowLong(hwnd, GWL_STYLE);

	if (dwStyle & WS_OVERLAPPEDWINDOW)
	{
		MONITORINFO monitorInfo = { sizeof(monitorInfo) };
		if (GetWindowPlacement(hwnd, &PrevWindowPlacement) &&
			GetMonitorInfo(MonitorFromWindow(hwnd,
			MONITOR_DEFAULTTOPRIMARY), &monitorInfo)) 
		{
			SetWindowLong(hwnd, GWL_STYLE,
				dwStyle & ~Window.WindowFlags);

			SetWindowPos(hwnd, HWND_TOP,
				monitorInfo.rcMonitor.left, monitorInfo.rcMonitor.top,
				monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left,
				monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top,
				SWP_NOOWNERZORDER | SWP_FRAMECHANGED);

			RECT windowRect;
			GetClientRect(Window.Window, &windowRect);
			Window.Width = windowRect.right;
			Window.Height = windowRect.bottom;
			SetViewPort(windowRect.left, windowRect.top, windowRect.right, windowRect.bottom);
		}
	}
	else 
	{
		SetWindowLong(hwnd, GWL_STYLE,
			dwStyle | Window.WindowFlags);

		SetWindowPlacement(hwnd, &PrevWindowPlacement);

		SetWindowPos(hwnd, NULL, 0, 0, 0, 0,
			SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
			SWP_NOOWNERZORDER | SWP_FRAMECHANGED);

		RECT windowRect;
		GetClientRect(Window.Window, &windowRect);
		Window.Width = windowRect.right;
		Window.Height = windowRect.bottom;
		SetViewPort(windowRect.left, windowRect.top, windowRect.right, windowRect.bottom);
	}
}

LRESULT CALLBACK Win32_Core::WindowCallBack(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
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

void Win32_Core::InitSystem(HINSTANCE hInstance, BasicGame *game, char *title, int width, int height, int ups)
{
	PrevWindowPlacement = { sizeof(PrevWindowPlacement) };
	Window = {};
	Window.WindowCallback = WindowCallBack;
	Window.WindowFlags = WS_OVERLAPPEDWINDOW ^ WS_MAXIMIZEBOX ^ WS_THICKFRAME;
	InitWindow(hInstance, &Window, title, width, height);
	Keys = {};
	InitInputManager(&Keys);	

	UPS = ups;

	Game = game;

	//Initialize SDL AUDIO
	if (SDL_Init(SDL_INIT_AUDIO) < 0)
	{
	std::cout << "SDL could not initialize! SDL Error: %s\n", SDL_GetError();
	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
	std::cout << "SDL_mixer could not initialize! SDL_mixer Error: \n";
	}

	Game->InitSystem();

	Game->Dimensions.Width = (float)Window.Width;
	Game->Dimensions.Height = (float)Window.Height;

	Game->Input.Exit = false;

	{
		Game->Input.UP.Button = VK_UP;
		Game->Input.DOWN.Button = VK_DOWN;
		Game->Input.RIGHT.Button = VK_RIGHT;
		Game->Input.LEFT.Button = VK_LEFT;
		Game->Input.Space.Button = VK_SPACE;
		Game->Input.Enter.Button = VK_RETURN;
		Game->Input.Escape.Button = VK_ESCAPE;

		Game->Input.W.Button = 'W';
		Game->Input.S.Button = 'S';
		Game->Input.A.Button = 'A';
		Game->Input.D.Button = 'D';
		Game->Input.P.Button = 'P';
		Game->Input.O.Button = 'O';
		Game->Input.Z.Button = 'Z';
		Game->Input.E.Button = 'E';

		Game->Input.MOUSE_LEFT.Button = MK_LBUTTON;
		Game->Input.MOUSE_MIDDLE.Button = MK_MBUTTON;
		Game->Input.MOUSE_RIGHT.Button = MK_RBUTTON;
	}

	Game->Init();
}

void Win32_Core::ProcessInput(Game_Input *input)
{
	for (uint32 i = 0; i < BUTTON_COUNT; i++)
	{
		if (i < BUTTON_COUNT - 3)
		{
			input->Buttons[i].KeyDown = IsKeyDown(&Keys, input->Buttons[i].Button);
			input->Buttons[i].KeyUp = IsKeyUp(&Keys, input->Buttons[i].Button);
		}
		else
		{
			input->Buttons[i].KeyDown = Keys.ButtonsDown[i - (BUTTON_COUNT - 3)];
			input->Buttons[i].KeyUp = Keys.ButtonsUp[i - (BUTTON_COUNT - 3)];
		}
	}

	if (IsAltF4Down(&Keys))
	{
		IsRunning = false;
	}

	if (IsAltEnterUp(&Keys))
	{
		SwitchFullScreen(Window.Window);
		Game->UpdateViewSize(Window.Width, Window.Height);
	}

	input->MousePos = vec2f(Keys.MouseX, Keys.MouseY);

	Keys.ButtonsUp[0] = false;
	Keys.ButtonsUp[1] = false;
	Keys.ButtonsUp[2] = false;
}

void Win32_Core::Run()
{
	IsRunning = true;
	MainLoop();
}

void Win32_Core::Stop()
{
	IsRunning = false;
}

void Win32_Core::Release()
{
	Game->ShutDown();
	wglMakeCurrent(NULL, NULL);
	ReleaseDC(Window.Window, GetDC(Window.Window));
	DestroyWindow(Window.Window);
}

LARGE_INTEGER Win32_Core::GetTicks()
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
float Win32_Core::GetSecondsElapsed(LARGE_INTEGER start, LARGE_INTEGER end)
{
	float result = ((float)(end.QuadPart - start.QuadPart)) / (float)TicksPerSecond;

	return result;
}

void Win32_Core::PrintTimeElapsed(LARGE_INTEGER start, LARGE_INTEGER end)
{
	float msPerFrame = 1000.0f * GetSecondsElapsed(start, end);

	float fps = 1000.0f / msPerFrame;

	std::cout<< "ms/f: " << msPerFrame << ", FPS: " << fps << "\n";
}

//Our main loop which should continue running as long as we don't quite the game
void Win32_Core::MainLoop()
{
	LARGE_INTEGER performanceFrequency;
	QueryPerformanceFrequency(&performanceFrequency);
	TicksPerSecond = performanceFrequency.QuadPart;

	float updateTime = 0;
	int updates = 0;
	int frames = 0;
	double frameTime = 0;

	bool createWindow = false;
	
	ClearColor color = ClearColor{ 0, 0, 0, 1 };
	SetClearColor(color);

	LARGE_INTEGER lastTick = GetTicks();
	float lag = 0;
	float frameCounter = 0;
	float delta = 1.0f / UPS;

	while (IsRunning)
	{
		LARGE_INTEGER current = GetTicks();
		float elapsed = GetSecondsElapsed(lastTick, current);
		lastTick = current;
		lag += elapsed;		
		frameCounter += elapsed;		

		while (lag >= delta)
		{
			ProcessPendingMessages(&Keys);
			ProcessInput(&Game->Input);
			Game->Update(delta);
			lag -= delta;
			updates++;
		}

		ClearWindow();
		
		Game->Render();

		RenderWindow(Window.Window);

		frames++;
		if (frameCounter > 1.0f)
		{
			std::cout << "FPS: " << frames << ", UPS: " << updates << std::endl;
			frameCounter = 0;
			frames = 0;
			updates = 0;
		}

		if (Game->Input.Exit)
		{
			IsRunning = false;
		}
	}

	Release();
}

void Win32_Core::CreateConsole()
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