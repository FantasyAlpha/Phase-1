#pragma once
#include"Window.h"

class Core
{
public:
	Core(){};
	~Core() {}

private:
	Window window;

	void update();

	void render();


public:

	void init(int width, int height, char *name);
	
	void run();


};

