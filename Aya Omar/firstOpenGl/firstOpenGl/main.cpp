// include the class header file 
#include "display.h"
#include "shader.h"
#include"mesh.h"

#include<iostream>
//to use open gl functions
#include<GL/glew.h>

int main() {
	vertex vertices[] = {
		vertex(glm::vec3(-.5,-.5 ,0)),
		vertex(glm::vec3(0, .5, 0)),
		vertex(glm::vec3(.5, -.5, 0)),
	};
	
	display d(800, 600, "hello world");
	mesh triangle(vertices, sizeof vertices / sizeof vertices[0]);

	// the string argument of the constructor must be const !!!
shader shader("res\\basicShader\\basicshader.vs", "res\\basicShader\\basicshader.fs");

	// caling the update while not closed 
	while (!d.isclosed()){
		glClear(GL_COLOR_BUFFER_BIT);
		d.clear(1,1,0,0);
	    shader.bind();
		triangle.draw();
		d.update();
	}

		//system("pause");

	return 0;

}