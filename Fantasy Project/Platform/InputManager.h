#pragma once

#include <Windows.h>
#include <Windowsx.h>

struct Input_Keys
{
	//Array holding the key down state
	bool KeysDown[200];
	//Array holding the key released state
	bool KeysUp[200];

	bool ButtonsDown[3];
	bool ButtonsUp[3];

	float MouseX, MouseY;
	unsigned int Key;
	unsigned int Char;
	
	bool AltF4;
};

//Initialize the input manager
void InitInputManager(Input_Keys *keys);

LRESULT CALLBACK WindowCallBack(HWND window, UINT message, WPARAM wParam, LPARAM lParam);
void ProcessPendingMessages(Input_Keys *keys);
//Get key down state
bool IsKeyDown(Input_Keys *keys, int keyCode);

//Get key released state
bool IsKeyUp(Input_Keys *keys, int keyCode);