#include "InputManager.h"

//Static members declaration
bool InputManager::m_KeysDown[512];
bool InputManager::m_KeysUp[512];

InputManager::InputManager()
{
}


InputManager::~InputManager()
{
}

//Initialize the input manager
void InputManager::InitInputManager(WindowManager *window)
{
	//Reset all the key states to false
	for (unsigned int KeyIndex = 0; KeyIndex < 512; KeyIndex++)
	{		
		m_KeysDown[KeyIndex] = false;
		m_KeysUp[KeyIndex] = false;
	}

	//Set the call back function 
	glfwSetKeyCallback(window->Window, KeyCallBack);
}

//Call back function 
void InputManager::KeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//If the event is (key press) set the key down state to true
	//and the key up state to false
	if (action == GLFW_PRESS)
	{		
		m_KeysDown[key] = true;
		m_KeysUp[key] = false;
	}

	//If the event is (key released) set the key down state to false
	//and the key up state to true
	if (action == GLFW_RELEASE)
	{
		m_KeysDown[key] = false;
		m_KeysUp[key] = true;
	}
}

//Get key down state
bool InputManager::IsKeyDown(int key)
{	
	return m_KeysDown[key];
}

//Get key released state
bool InputManager::IsKeyUp(int key)
{
	//Store the state in a separate variable and reset it in the array
	bool b = m_KeysUp[key];
	m_KeysUp[key] = false;
	return b;
}
