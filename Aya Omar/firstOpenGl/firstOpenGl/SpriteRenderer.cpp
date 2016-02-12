#include "SpriteRenderer.h"


//SpriteRenderer::SpriteRenderer(shader & m_shader){
//	this->shader = m_shader;
//	initRenderMesh();
//}

SpriteRenderer::SpriteRenderer(shader &shader) {

	this->m_shader = shader;
	this->initRenderMesh();
}

SpriteRenderer:: ~SpriteRenderer() {

}

void SpriteRenderer::initRenderMesh()
{
    
	// intilaize our mesh with vertex (pos , textcoord)
	//vertices from 0 ===>1 map all the of the texture to the sprite rectangle  
	vertex vertices[6] = {
		vertex(glm::vec3(0.0f, 1.0f, 0), glm::vec2(0.0, 1.0)),
		vertex(glm::vec3(1.0, 0.0f, 0), glm::vec2(1.0, 0)),
		vertex(glm::vec3(0.0f, 0.0f, 0), glm::vec2(0, 0)),

		vertex(glm::vec3(0.0f, 1.0f, 0), glm::vec2(0.0, 1.0)),
		vertex(glm::vec3(1.0, 1.0f, 0), glm::vec2(1.0, 1.0)),
		vertex(glm::vec3(1.0f, 0.0f, 0), glm::vec2(1, 0)),
	};

	sprite =  mesh(&vertices[0], sizeof vertices / sizeof vertices[0]);

	
}


// the following function will do all the rendering stuff 
// which will be called in game class rendere function 
//1- bind the shader
//2-transformations
// 3- bind texture
//4-call mesh :: draw to draw the sprite on the screen 
transform SpriteRenderer::gettransform(glm::vec2 position, glm::vec2 size ) {
	// 2- Prepare transformations

	transform.getPos() = glm::vec3(position, 0.0f);
	transform.getRot() = glm::vec3(0, 0, 1);
	transform.getScale() = glm::vec3(size, 1.0f);
	return transform;
}
void SpriteRenderer::DrawSprite(/*Texture &texture, */glm::vec2 position, glm::vec2 size, GLfloat rotate, glm::vec3 color)
{
	// 1- use the shader 

	//this->shader.bind();

	
	//do transform and projection uniforms 
	//shader.updateTransform(transform);

	// 3-bind texture 
	//texture.Bind();

	//4- draw sprite 
	
	sprite.draw();

	//glm::mat4 model;
	//model = glm::translate(model, glm::vec3(position, 0.0f));  // First translate (transformations are: scale happens first, then rotation and then finall translation happens; reversed order)

	//model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); // Move origin of rotation to center of quad
	//model = glm::rotate(model, rotate, glm::vec3(0.0f, 0.0f, 1.0f)); // Then rotate
	//model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); // Move origin back

	//model = glm::scale(model, glm::vec3(size, 1.0f)); // Last scale

	//this->shader.SetMatrix4("model", model);

	//// Render textured quad
	//this->shader.SetVector3f("spriteColor", color);

	
}








