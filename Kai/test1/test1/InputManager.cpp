#include "InputManager.h"

//Static members declaration
bool InputManager::m_KeysDown[512];
bool InputManager::m_KeysUp[512];

#if MODE_TYPE == SDL_MODE
SDL_Event InputManager::Event;
#endif

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

	//Set the call back function (GLFW mode only)
#if MODE_TYPE == GLFW_MODE
	glfwSetKeyCallback(window->Window, KeyCallBack);
#endif
}

//Call back function (GLFW mode only)
#if MODE_TYPE == GLFW_MODE
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
//Polling events (SDL mode only)
#elif MODE_TYPE == SDL_MODE
void InputManager::Update(WindowManager *window)
{
	//While there are any events in the queue ,process them
	while (SDL_PollEvent(&m_Event))
	{
		//If the user requested closing the window 
		//,then the Should close flag in window manager is set to true
		if (m_Event.type == SDL_QUIT)
		{
			window->ShouldClose = true;
		}

		//If the event is (key down) set the key down state to true
		//and the key up state to false
		if (m_Event.type == SDL_KEYDOWN)
		{
			int value = m_Event.key.keysym.scancode;

			m_KeysDown[value] = true;
			m_KeysUp[value] = false;
		}

		//If the event is (key up) set the key down state to false
		//and the key up state to true
		if (m_Event.type == SDL_KEYUP)
		{
			int value = m_Event.key.keysym.scancode;

			m_KeysDown[value] = false;
			m_KeysUp[value] = true;
		}
	}
}

#endif

//Get key down state
bool InputManager::IsKeyDown(unsigned int key)
{	
	return m_KeysDown[key];
}

//Get key released state
bool InputManager::IsKeyUp(unsigned int key)
{
	//Store the state in a separate variable and reset it in the array
	bool b = m_KeysUp[key];
	m_KeysUp[key] = false;
	return b;
}
