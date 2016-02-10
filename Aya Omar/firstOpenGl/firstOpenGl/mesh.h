#pragma once
// include math library to use  (x,y,z) coordinates 
#include <glm/glm.hpp>
#include<GL/glew.h>

// create class for the vertex type 
class vertex {

public :
	vertex(const glm::vec3& position, const glm::vec2& textCoord){

		this->position = position;
		this->textCoord = textCoord;
	}
	 glm :: vec3 *getpos() { return &position; }
	 glm::vec2 *gettextCoord() { return &textCoord; }



private: 
	glm::vec3 position;
	glm::vec2 textCoord;


};


class mesh
{
public:
	// constructor must take a pointer of data or vertices start position 
	mesh(vertex *vertices ,unsigned int numVertices);
	virtual ~mesh();
	void draw();

private :
	// the vertices are data in the gpu so open gl will help to get it with the handel as follow
	GLuint m_vertexArrayObject;
	// point to the buffer in the GPU  hold the vertex array which form the triangle we wnat to draw 
	enum {
		// handel of the buffer and num
		pos_vB ,
		texCoord_vB,
		num_buffers
	};
	GLuint m_ArrayBuffers[num_buffers];
	// array of buffers handels in case we want  more than one data buffer for more than one triangle or mesh 

	// count how many data we want to draw 
	unsigned int m_drawCount;



};

