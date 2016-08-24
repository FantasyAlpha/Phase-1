#pragma once

#include "InputManager.h"
#include "WindowManager.h"
#include <BasicGame.h>
#include <stdint.h>
//

struct Win32_Core
{
public:
	Win32_Core(){}
	~Win32_Core(){}
public:
	static Win32_Core* CreateCore();

	void Run();

	static LRESULT CALLBACK WindowCallBack(HWND window, UINT message, WPARAM wParam, LPARAM lParam);

	void InitSystem(HINSTANCE hInstance, BasicGame *game, char *title, int width, int height, int ups);

	void CreateConsole();

private:
	void Stop();

	void Release();

	void ProcessInput(Game_Input *input);

	void MainLoop();

	float GetSecondsElapsed(LARGE_INTEGER start, LARGE_INTEGER end);

	void PrintTimeElapsed(LARGE_INTEGER start, LARGE_INTEGER end);

	LARGE_INTEGER GetTicks();	

	void SwitchFullScreen(HWND hwnd);

private:
	BasicGame *Game;

	Input_Keys Keys;
	int64_t TicksPerSecond;
	float UPS;
	WindowManager Window;
	WINDOWPLACEMENT PrevWindowPlacement;
	static bool IsRunning;
	static Win32_Core *Core;
};
