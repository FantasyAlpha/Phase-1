#include "Core.h"


void Core::run()
{
	while (!window.shouldClose())
	{
		update();
		render();
	}

	//WARNING!! destroy after escaping the loop
	window.destroy();
}

void Core::init(int width, int height, char *name)
{
	window = Window(width, height, name);
	window.createWindow();
}

void Core::update()
{
	window.update();
}

void Core::render()
{
	window.render();

}
