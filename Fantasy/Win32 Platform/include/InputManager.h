#pragma once

#include <Windows.h>
#include <Windowsx.h>

#define MAX_KEYS 512

struct Input_Keys
{
	bool KeysDown[MAX_KEYS];
	bool KeysUp[MAX_KEYS];

	bool ButtonsDown[3];
	bool ButtonsUp[3];

	float MouseX, MouseY;
	unsigned int Key;
	unsigned int Char;
	
	bool AltF4;
	bool AltEnter;
};

void InitInputManager(Input_Keys *keys);

void ProcessPendingMessages(Input_Keys *keys);

bool IsKeyDown(Input_Keys *keys, unsigned int keyCode);
bool IsKeyUp(Input_Keys *keys, unsigned int keyCode);

bool IsAltEnterUp(Input_Keys *keys);

bool IsAltF4Down(Input_Keys *keys);