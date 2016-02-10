// include the class header file 
#include "display.h"
#include "shader.h"
#include"mesh.h"
#include "transform.h"
#include"Game.h"
#include"Texture.h"
#include<iostream>
//to use open gl functions
#include<GL/glew.h>



int main() {
	//default constructor 
	Game Breakout;
	// OpenGL configuration
	glViewport(0, 0, 800, 600);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// Initialize game
	Breakout.Init();

	/*DeltaTime variables
	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = 0.0f;*/

	// Start Game within Menu State
	 Breakout.State = GAME_ACTIVE;

	vertex vertices[] = {
		vertex(glm::vec3(-0.5f, -0.5f, 0), glm :: vec2 (0,0)),
		vertex(glm::vec3(0, 0.5f, 0), glm::vec2(0.5, 1)),
		vertex(glm::vec3(0.5f, -0.5f, 0), glm::vec2(1, 0)),
	};

	display d(800, 600, "Break Out");

	mesh triangle(vertices, sizeof vertices / sizeof vertices[0]);
	// the string argument of the constructor must be const !!!
	shader shader("res\\basicShader\\basicshader,vs.txt", "res\\basicShader\\basicshader,fs.txt");
	Texture tex("res\\basicShader\\bricks .jpg") ;
	// IMP : calling the default constructor 
	transform transform;
	float counterTransform = 0.0f;

	// caling the update while not closed 

	while (!d.isclosed()){
		
		d.clear(0, 0, 1, 0);
		
		shader.bind();

		// cause get function return by refernce vec 3 we can access each position x , y ,z 
		transform.getPos().x = sinf(counterTransform / 100);
		//transform.getPos().y = sinf(counterTransform / 100);
		transform.getRot().z = counterTransform / 100;
		//transform.getRot().x = counterTransform * 10;
	//	transform.getScale().y = counterTransform / 1000;
		//transform.getScale().x = counterTransform / 1000;

		shader.updateTransform(transform);

		triangle.draw();
		//update contain render and handel input stuff

		// game class functions 
		Breakout.ProcessInput();
		Breakout.Update();
		Breakout.Render();

		d.update();
		counterTransform +=.1;
	}

	system("pause");

	return 0;

}