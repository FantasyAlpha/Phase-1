#include "WindowManager.h"

//Sets some options for opengl
void WindowManager::SetOpenglOptions()
{
	//The color that we will use to clear the screen (painting it with this color)
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

//NOTE(kai): a context is basically all data within the window ,including attributes, buffers, ...etc
//Initialize a window
void WindowManager::InitWindow(char *title, int width, int height)
{
	//create a window by using GLFW
#if MODE_TYPE == GLFW_MODE
	//NOTE(kai): we have to :
	//		1) initialize GLFW
	//		2) set the wanted attributes
	//		3) create the window
	//		4) prepare the context (by setting some attributes) then use it with our window

	//Initialize GLFW library and check if the initialization fails
	if (!glfwInit())
	{
		std::cout << "Failed to initialize GLFW \n";
		system("PAUSE");
	}

	//Set the window's resize attribute
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	//Creates a window
	Window = glfwCreateWindow(width		//Window width
		, height	//Window height
		, title		//Window title
		, NULL		//The monitor to use for fullscreen mode (NULL means windowed)
		, NULL);	//Another window whose context will be shared with this one 

	//Check if window is created successfully
	if (!Window)
	{
		std::cout << "Failed to create window \n";
		system("PAUSE");
	}

	//Sets the highest and lowest opengl versions that the window can support
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	//Specify which opengl profile we will be using (ANY_PROFILE allows us to support opengl versions below 3.2)
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
	//Set the current context the this window
	glfwMakeContextCurrent(Window);

#elif MODE_TYPE == SDL_MODE		//create a window by using SDL
	//NOTE(kai): we have to :
	//		1) initialize SDL
	//		2) set the wanted attributes
	//		3) create the window
	//		4) create a context 

	//Initialize all systems in the SDL library
	SDL_Init(SDL_INIT_EVERYTHING);
	//Whether to use a single or double buffer
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, SDL_TRUE);

	Window = SDL_CreateWindow(title					//Window title
							, SDL_WINDOWPOS_CENTERED	//Window X position
							, SDL_WINDOWPOS_CENTERED	//Window Y position
							, width					//Window width
							, height				//Window height
							, SDL_WINDOW_OPENGL); //Window usable with opengl context

	if (!Window)
	{
		std::cout << "Failed to create window \n";
		system("PAUSE");
	}

	SDL_GLContext openglContext = SDL_GL_CreateContext(Window);

	if (openglContext == nullptr)
	{
		std::cout << "Failed to create opengl context \n";
		system("PAUSE");
	}

#endif
	//NOTE(kai): initialize opengl using GLEW

	//Initialize GLEW library and check if the initialization fails
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW \n";
		system("PAUSE");
	}

	//NOTE(kai): Since GLEW obtains information about the supported extensions (opengl features) from the graphics driver 
	//,however some of the experimental or prerelease drivers might not report these extension
	//, so by using the glewExperimental switch we can obtain these hidden extensions 
	glewExperimental = GL_TRUE;

	//Print out the opengl version supported by our driver
	PrintOpenglVersion();

	//Sets the portion of the window we will render on
	glViewport(0		//The X origin
		, 0		//The Y origin
		, width		//Width of the portion we will render on
		, height);		//Height of the portion we will render on

	//Sets some options for opengl
	SetOpenglOptions();

	//Print out what library we are using
	if (MODE_TYPE == GLFW_MODE)
	{
		std::cout << "We are using GLFW\n";
	}
	else if (MODE_TYPE == SDL_MODE)
	{
		std::cout << "We are using SDL\n";
	}
}

//Renders our window (this should happen each frame)
void WindowManager::RenderWindow()
{
	//Print any opengl error during rendering
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		std::cout << "OpenGL Error: " << error << std::endl;
	}	
	
	//Swap front and back buffers
#if MODE_TYPE == GLFW_MODE
	glfwSwapBuffers(Window); //Swap buffers in glfw
#elif MODE_TYPE == SDL_MODE
	SDL_GL_SwapWindow(Window);	//Swap buffers in glfw
#endif
}

//Updates our window (this should happen each frame)
void WindowManager::UpdateWindow()
{
	//Process the events in the event queue
#if MODE_TYPE == GLFW_MODE
	glfwPollEvents();	//process the events using GLFW
#endif
}


//See if the user wants to close the window
bool WindowManager::IsCloseRequested()
{
	//Get the window close flag
#if MODE_TYPE == GLFW_MODE
	return glfwWindowShouldClose(Window);
#elif MODE_TYPE == SDL_MODE
	return ShouldClose;
#endif
}

//Destroy the library (which results in destroying the window)
void WindowManager::DestroyWindow()
{
#if MODE_TYPE == GLFW_MODE
	glfwTerminate();	//Destroy GLFW
#elif MODE_TYPE == SDL_MODE
	SDL_Quit();		//Destroy SDL
#endif
}

//Clear the window
void WindowManager::Clear()
{
	//Clears the window with the clear color we specified before (we have to specify which buffers we want to clear)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}