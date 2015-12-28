
#include "Core.h"

//void inputHandle(GLFWwindow *window, int key, int scan, int action, int mode)
//{
//	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
//	{
//		glfwSetWindowShouldClose(window, true);
//	}
//}


int main()
{	
	Core core;
	core.init(800, 600, "test");
	core.run();

	system("PAUSE");
	return 0;
}