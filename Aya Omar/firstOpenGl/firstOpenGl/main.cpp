// include the class header file 
#include "display.h"

//to use open gl functions
#include<GL/glew.h>

int main() {

	display d(800, 600, "hello world");
	
	// caling the update while not closed 
	while (!d.isclosed()){
		
		d.clear(1,1,0,0);

		d.update();
	}

		//system("pause");

	return 0;

}