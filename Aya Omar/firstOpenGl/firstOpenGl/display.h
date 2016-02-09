#pragma once
// string declration heere in header file
#include<string>
//#include<Windows.h>


// this class  will implement  :

// 1- sdl and glew intializations 
// 2- creating window (taking parameters of window from the main )
//3- create context
//4- handel window rendering stuff (swap buffers , clear color)
//5- check events (window quit , keys presssed / released)


#include<SDL2/SDL.h>
// SDL_main.h is included automatically from SDL.h, so you always get the nasty #define.

#undef main
class display
{
public:
	display(int weidth , int height , std :: string title);
	void update();
	bool isclosed();
	void clear(float r, float g, float b, float a);
	virtual~display();

private :
	// variable to hold the window pointer
	SDL_Window* m_window;
	// holding the open gl context ( makes GPU talk to the window )
	SDL_GLContext m_glContext;
	bool m_closed;
	
};

