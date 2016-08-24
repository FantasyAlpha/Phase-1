#include <Core.h>
#include <TestGame.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdShow)
{
	Win32_Core Core = Win32_Core();
	Core.CreateConsole();	
	Core.InitSystem(hInstance, new TestGame(), "Kai", 1280, 720, 60);
	Core.Run();

	return 0;
}