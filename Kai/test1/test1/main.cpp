/*=======================================================================================================================
NOTE(kai): This file can:
		1) Intialize the engine
		2) run the engine
=======================================================================================================================*/

#include "EngineCore.h"

#undef main //To be able to use SDL because it defines it's own main

int main()
{
	//Instance of the engine's core 
	EngineCore *engine = new EngineCore("Kai engine", 1280, 720);
	
	//Start the engine
	engine->Run();

	return 0;
}