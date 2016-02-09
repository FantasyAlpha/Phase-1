#include "display.h"
#include<iostream>
#include<GL/glew.h>

display::display(int width, int height, std::string title)
{
	std::cout << "constructor"<<std ::endl;

	//intialize the sdl to handel os stuff
	SDL_Init(SDL_INIT_EVERYTHING);

	// setting attributes for the created window 

	//RGB pixels 
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	//transparent factor 
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	// pixel size = 8*4 =32 bits
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);

	//  ** we need two buffers for windows or 2 memory spaces 1- display what we dew  2- to draw with open gl 
	// we cant display what is in the process of drawing so  we use the 2 and then (((swap))) beween them 
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// 1===> creating the window 

	m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
	
	//creating open gl context which can make GPU deal with the window (reduce OS control on the window)

	m_glContext = SDL_GL_CreateContext(m_window);

	// we need to intilaize glew to acceess open gl functions 
	GLenum status = glewInit();
	// check that intializing is okay

	if (status != GLEW_OK){
		//cerr awl mara ashofha :D :D
		std::cerr << " error intializing glew " << std::endl;
	}
	m_closed = false;
}


display::~display()
{
	//  cout : just to check the destructor works
	std::cout << "destructor"<<std::endl;
	// delete context fiiirst then===> the window 

	SDL_GL_DeleteContext(m_glContext);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}

// function update : updating and handling the display 
void display::update() {


	// 1====> swaping the double buffer functions 
	SDL_GL_SwapWindow(m_window);

	// declare sdl event
	SDL_Event e;
	// searching for sdl events 
	while (SDL_PollEvent(&e)) {
		
		//2====> check if the event is window quit or closed 
		if (e.type == SDL_QUIT)
		{
			m_closed = true;
		}

		// check key boared key preesed event handeled by the SDL library 

		else if (e.type ==SDL_KEYDOWN)
		{
			// check the symbol pressed 
			switch (e.key.keysym.sym) {
				//all keyboard symbols available in sdlk (key)
			case SDLK_x:
				// check the symbol with printing 

				// lazem break after each case -_- -_- 

				std::cout << "the key entered is X" << std::endl;
				break;
			case SDLK_y:
				std::cout << "the key entered is Y" << std::endl;
				break;
			case SDLK_z:
				std::cout << "the key entered is Z" << std::endl;
				break;
			case SDLK_SPACE:
				std::cout << "the key entered is Space " << std::endl;
				break;
			case SDLK_ESCAPE :
				SDL_Quit();
				break;

			}
		
		}
		// check mouse pressed 
		else if (e.type == SDL_MOUSEBUTTONDOWN) {
			{
				if (e.button.button == SDL_BUTTON_LEFT)
				std::cout << "left mouse clicked" << std::endl;
			}
		}

	}

}
bool display::isclosed() {
	return m_closed;
}

void display::clear(float r, float g, float b, float a){
	// coloring the windowww

	glClearColor(r,g,b,a);
	glClear(GL_COLOR_BUFFER_BIT);

}
