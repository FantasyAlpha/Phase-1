// include the class header file 
#include "display.h"
#include "shader.h"
#include"mesh.h"

#include<iostream>
//to use open gl functions
#include<GL/glew.h>
#include "transform.h"

int main() {
	vertex vertices[] = {
		vertex(glm::vec3(-0.5f, -0.5f, 0)),
		vertex(glm::vec3(0, 0.5f, 0)),
		vertex(glm::vec3(0.5f, -0.5f, 0)),
	};

	display d(800, 600, "hello world");
	mesh triangle(vertices, sizeof vertices / sizeof vertices[0]);
	// the string argument of the constructor must be const !!!
	shader shader("res\\basicShader\\basicshader,vs.txt", "res\\basicShader\\basicshader,fs.txt");
	// IMP : calling the default constructor 
	transform transform;
	float counterTransform = 0.0f;
	// caling the update while not closed 

	while (!d.isclosed()){
		
		d.clear(0, 0, 1, 0);
		// cause get function return by refernce vec 3 we can access each position x , y ,z 
		transform.getPos().x= sinf(counterTransform/100);
		//transform.getPos().y = sinf(counterTransform / 100);
		transform.getRot().z = counterTransform /100;
		//transform.getRot().x = counterTransform * 10;
		transform.getScale().y = counterTransform /1000;
		transform.getScale().x = counterTransform /1000;


		shader.bind();
		shader.updateTransform(transform);
		triangle.draw();

		d.update();
		counterTransform +=.1;
	}

	system("pause");

	return 0;

}