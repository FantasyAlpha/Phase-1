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

UINT VirtualkeyToScancode(WPARAM wParam, LPARAM lParam)
{
	if (HIWORD(lParam) & 0x0F00)
	{
		UINT scancode = MapVirtualKey(wParam, 0);
		return scancode | 0x80;
	}
	else
	{
		return HIWORD(lParam) & 0x00FF;
	}
}

int ScancodeToAscii(UINT code, unsigned short *result)
{
	
	HKL layout = GetKeyboardLayout(0);
	unsigned char state[256];
	if (GetKeyboardState(state) == FALSE)
	{
		return 0;
	}

	UINT vk = MapVirtualKeyEx(code, 1, layout);

	return ToAsciiEx(vk, code, state, result, 0, layout);
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
					keys->Key = key;
				}

				bool altWasDown = (msg.lParam & (1 << 29));
				if ((key == VK_F4) && altWasDown)
				{				
					keys->AltF4 = true;
				}
			}
			case WM_CHAR:
			{
				unsigned int key = msg.wParam;
				bool wasDown = ((msg.lParam & (1 << 30)) != 0);
				bool isDown = ((msg.lParam & (1 << 31)) == 0);

				if (isDown && !wasDown)
				{
				}
				keys->Char = key;
			}
			break;	


			case WM_LBUTTONDOWN:
			{
				keys->ButtonsDown[0] = true;
				keys->ButtonsUp[0] = false;
			}
			break;

			case WM_MBUTTONDOWN:
			{
				keys->ButtonsDown[2] = true;
				keys->ButtonsUp[2] = false;
			}
			break;

			case WM_RBUTTONDOWN:
			{
				keys->ButtonsDown[1] = true;
				keys->ButtonsUp[1] = false;
			}
			break;

			case WM_LBUTTONUP:
			{
				keys->ButtonsDown[0] = false;
				keys->ButtonsUp[0] = true;
			}
			break;

			case WM_MBUTTONUP:
			{
				keys->ButtonsDown[2] = false;
				keys->ButtonsUp[2] = true;
			}
			break;

			case WM_RBUTTONUP:
			{
				keys->ButtonsDown[1] = false;
				keys->ButtonsUp[1] = true;
			}
			break;

			case WM_MOUSEMOVE:
			{
				keys->MouseX = GET_X_LPARAM(msg.lParam);
				keys->MouseY = GET_Y_LPARAM(msg.lParam);
			}
			break;

			default:
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);			
			}
			break;
		}
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
