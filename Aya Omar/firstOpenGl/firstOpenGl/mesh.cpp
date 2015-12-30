#include "mesh.h"
#include<iostream>
#include<GL/glew.h>

mesh::mesh(vertex *vertices, unsigned int numVertices)
{
	
	// say num of triangles i want to draw (each triangle is array of vertices )
	m_drawCount = numVertices;
	glewExperimental = GL_TRUE;
	glewInit();
	GLenum status = glewInit();
	// check that intializing is okay

	if (status != GLEW_OK){
		//cerr awl mara ashofha :D :D
		std::cerr << " error intializing glew " << std::endl;
	}
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

	glBufferData(GL_ARRAY_BUFFER,numVertices *sizeof vertices[0],vertices,GL_STATIC_DRAW);

	// need to tell open gl the attribute of the vertex we sending vec3 pos , vec3 color ,......
	// opengl must know that this data attribute like pos  is sequental 

	glEnableVertexAttribArray(0); //?????????
	// each attribute consists of 3 peaces and their type , false : dont skip any attributes they are sequential , start at pos 0
	glVertexAttribPointer(0, 3,GL_FLOAT,GL_FALSE,0,0);

	// finalllly enough infoooo for open gl to drawww







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