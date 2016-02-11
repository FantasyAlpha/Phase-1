#include "mesh.h"
#include<iostream>
#include<GL/glew.h>
#include<vector>

mesh::mesh(vertex *vertices, unsigned int numVertices) 
{
	
	m_drawCount = numVertices;

	std::vector <glm::vec3> positions;
	std::vector <glm::vec2> textCoord;
	positions.reserve(numVertices);
	textCoord.reserve(numVertices);
	for (unsigned int i = 0; i < numVertices; i++) {

		positions.push_back(*vertices[i].getpos());
		textCoord.push_back(*vertices[i].gettextCoord());

	}
	// say num of triangles i want to draw (each triangle is array of vertices )
	
	glGenVertexArrays(1, & m_vertexArrayObject);
	//Now we created space for the vertex array 

	// to use this space ====> BIND =====> operations (draw,....)======> un BIND

	glBindVertexArray(m_vertexArrayObject);

	// generate buffer space where we we store data in GPU

	glGenBuffers(num_buffers, m_ArrayBuffers);

	// start use it =====> bind

	// to use buffers specify which buffer of the array u will use and its type 
	glBindBuffer(GL_ARRAY_BUFFER, m_ArrayBuffers[pos_vB]);

	// finaaaaalllly ====> prepare buffer to hold the data 
	// following parameters  1- type of data in buffer   2- size of data   3- pointer of start of data 
	// 4- draw hint (tell GPU hint about the type of draw (need to change data or just draw ,........))

	glBufferData(GL_ARRAY_BUFFER,numVertices *sizeof positions[0],&positions[0],GL_STATIC_DRAW);

	// need to tell open gl the attribute of the vertex we sending vec3 pos , vec3 color ,......
	// opengl must know that this data attribute like pos  is sequental 

	glEnableVertexAttribArray(0); //the first attribute of the vertix ===> position 
	// each attribute consists of 3 peaces and their type , false : dont skip any attributes they are sequential , start at pos 0
	glVertexAttribPointer(0, 3,GL_FLOAT,GL_FALSE,0,0);

	// finalllly enough infoooo for open gl to drawww
	glBindBuffer(GL_ARRAY_BUFFER, m_ArrayBuffers[texCoord_vB]);
	glBufferData(GL_ARRAY_BUFFER, numVertices *sizeof textCoord[0], &textCoord[0], GL_STATIC_DRAW);
	//the second  attribute of the vertix ===> texCoord 
	glEnableVertexAttribArray(1);
	// 2 floating elements x , y
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);









	//unbind 
	glBindVertexArray(0);
}


mesh::~mesh()
{
	glDeleteVertexArrays(1, &m_vertexArrayObject);
}
	
void mesh::draw(){

	glBindVertexArray(m_vertexArrayObject);

	glDrawArrays(GL_TRIANGLES,0,m_drawCount);
	
	glBindVertexArray(0);


}