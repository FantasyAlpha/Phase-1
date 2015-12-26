
#include "SDL.h"


// Opening window fn.
 void openWindow ()
{
	//1- Initializing the video component. 
   SDL_Init( SDL_INIT_VIDEO );

   //2- Creating window.
   SDL_Surface* screen = SDL_SetVideoMode( 640 , 480 , 0, SDL_HWSURFACE | SDL_DOUBLEBUF );

   // 3- window's title.
   SDL_WM_SetCaption( "WELCOME :) " , 0 );

   SDL_Event event;
   bool gameRunning = true;

   // Keeping the Window Open until close it(by two ways ).
   while (gameRunning)
   {
      if (SDL_PollEvent(&event))
      {
		   // close the window.
         if (event.type == SDL_QUIT)
         {
			 
            gameRunning = false;
         }

		 // OR close it by useing Esc
		 if (event.type == SDL_KEYDOWN)
         {
            SDLKey keyPressed = event.key.keysym.sym;
      
            switch (keyPressed)
            {
               case SDLK_ESCAPE:
                  gameRunning = false;
                  break;
            }
         }
      } 
   }

   // Shutdown SDL 
   SDL_Quit();
}


int main(int argc, char **argv)
{
	// Calling fn to open window. 
	openWindow();


	// Getting input 
   return 0;
}


