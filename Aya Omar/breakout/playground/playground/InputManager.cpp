#include "InputManager.h"

//Initialize the input manager
void InitInputManager(Input_Keys *keys)
{
	//Reset all the key states to false
	for (unsigned int KeyIndex = 0; KeyIndex < 512; KeyIndex++)
	{		
		keys->KeysDown[KeyIndex] = false;
		keys->KeysUp[KeyIndex] = false;
		keys->AltF4 = false;
	}
}

void ProcessPendingMessages(Input_Keys *keys)
{
	MSG msg;
	while (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
	{
		switch (msg.message)
		{
			case WM_SYSKEYDOWN:
			case WM_SYSKEYUP:
			case WM_KEYDOWN:
			case WM_KEYUP:
			{
				unsigned int key = msg.wParam;
				bool wasDown = ((msg.lParam & (1 << 30)) != 0);
				bool isDown = ((msg.lParam & (1 << 31)) == 0);

				//If the event is (key press) set the key down state to true
				//and the key up state to false
				if (isDown && !wasDown)
				{
					keys->KeysDown[key] = true;
					keys->KeysUp[key] = false;
				}

				//If the event is (key released) set the key down state to false
				//and the key up state to true
				if (wasDown && !isDown)
				{
					keys->KeysDown[key] = false;
					keys->KeysUp[key] = true;
				}

				if ((key == VK_F4) && (msg.lParam & (1 << 29)))
				{				
					keys->AltF4 = true;
				}
			}
			break;	
			
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

//Get key down state
bool IsKeyDown(Input_Keys *keys, int key)
{	
	return keys->KeysDown[key];
}

//Get key released state
bool IsKeyUp(Input_Keys *keys, int key)
{
	//Store the state in a separate variable and reset it in the array
	bool b = keys->KeysUp[key];
	keys->KeysUp[key] = false;
	return b;
}
