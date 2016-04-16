#pragma once

#include <windows.h>
#include "ImGUI\imgui.h"
#include <GL\glew.h>
#include <Shader.h>
#include <Material.h>
#include <Types.h>
#include <Transform.h>

struct GUI_Button
{
	int Button;

	bool KeyDown;
	bool KeyUp;
};

#define GUI_BUTTON_COUNT 8

struct GUI_Input
{
	union
	{
		GUI_Button Buttons[GUI_BUTTON_COUNT];

		struct
		{		
			GUI_Button UP;
			GUI_Button DOWN;
			GUI_Button RIGHT;
			GUI_Button LEFT;
			GUI_Button BACKSPACE;
		
			GUI_Button MOUSE_LEFT;
			GUI_Button MOUSE_RIGHT;
			GUI_Button MOUSE_MIDDLE;
		};
	};
	GUI_Button Key;
	uint32 Char;
	float MouseX, MouseY;
};

struct GUI_Data
{
	Shader MainShader;
	
	uint32 VBO, IBO;
	uint32 TextureHandle;
};

static GUI_Data GUIData;

void GUI_Render(ImDrawData *drawData);

void GUI_BindBuffers();

void GUI_Init(float width, float height, GUI_Input *input);

void GUI_ProcessInput(GUI_Input *input);

void GUI_NewFrame();

//TODO(Kai):
//--> 1) Check getting the mouse position ==> DONE
//--> 2) Check if blend, depth-test, scissor-test,..etc are supported in opengl 2.1
//--> 3) Move the functions' implementations in the cpp file ==> DONE
//--> 4) Compile and link the output to the other projects ==> DONE
//--> 5) Add ImGUI to the mainloop ==> DONE
//--> 6) Try to create a button ==> DONE
//--> 7) Add GUI buttons to ProcessInput in main ==> DONE
//--> 8) Fix the input errors in gui
//--> 9) 

