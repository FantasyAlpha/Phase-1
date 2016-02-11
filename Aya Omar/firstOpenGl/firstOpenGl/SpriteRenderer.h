#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Texture.h"
#include "shader.h"
#include "mesh.h"
#include "transform.h"

class SpriteRenderer
{
public:
	//  (inits shaders/shapes)
	SpriteRenderer() {}

	SpriteRenderer(shader& shader);

	// Destructor
	~ SpriteRenderer();
	// Renders a defined quad textured with given sprite
	// 1- texture  2- sprite position x,y 3- size in x, y 4- color 5- rotate 
	transform  gettransform(glm::vec2 position, glm::vec2 size);

	void initRenderMesh();

	void DrawSprite( /*Texture &texture2d,*/ glm::vec2 position, glm::vec2 size = glm::vec2(10, 10), GLfloat rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));
private:
	// Render state


	shader shader;

	

	
	mesh sprite;

	transform transform;
	
	
	// Initializes and configures the mesh 
};

#endif