#pragma once

#include <Cinder\CinderMath.h>

struct Game_Button
{
	int Button;

	bool KeyDown;
	bool KeyUp;
};

#define BUTTON_COUNT 18
struct Game_Input
{
	union
	{
		Game_Button Buttons[BUTTON_COUNT];
		struct
		{
			Game_Button UP;
			Game_Button DOWN;
			Game_Button RIGHT;
			Game_Button LEFT;
			Game_Button Space;
			Game_Button Enter;
			Game_Button Escape;
			Game_Button W;
			Game_Button S;
			Game_Button A;
			Game_Button D;
			Game_Button Z;
			Game_Button P;
			Game_Button O;
			Game_Button E;
			Game_Button MOUSE_LEFT;
			Game_Button MOUSE_RIGHT;
			Game_Button MOUSE_MIDDLE;
		};
	};
	vec2f MousePos = vec2f();

	bool Exit;
};

struct Window_Dimensions
{
	float Width;
	float Height;
};
