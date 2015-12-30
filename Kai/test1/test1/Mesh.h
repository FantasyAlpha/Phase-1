#pragma once

#include <GL\glew.h>
#include <iostream>
#include "Vertex.h"

class Mesh
{
public:
	//Default constructor
	Mesh() = default;
	
	//This constructor loads the mesh automatically
	Mesh(Vertex *vertices, unsigned int verticesCount, unsigned int *indices, unsigned int indicesCount);
	~Mesh(){}
public:
	//Draw the mesh
	void Draw();
private:	
	//Create the buffers and store the mesh data in them
	void LoadMesh(Vertex *vertices, unsigned int *indices);

	//Create a Vertex buffer and stores the vertices in it 
	void InitVBO(Vertex *vertices);

	//Create an Element buffer and stores the indices in it 
	void InitEBO(unsigned int *indices);

	//NOTE(kai): only one buffer of the same type can be bound at a certain time

	//Bind the buffers
	void Bind();

	//Unbind the buffers
	void Unbind();
private:
	//NOTE(kai): vertices are the positions of the each vertex in the mesh
	//			,indices are the order of the vertices that i want to draw in

	//Handle to a special type object that stores a reference to the vertex buffer ,index buffer
	//	and the layout specification to the vertex itself 
	unsigned int m_VAO;		//Vertex array object

	//The Buffers' handles i will use to store the mesh data in
	unsigned int m_VBO;		//Vertex buffer 
	unsigned int m_EBO;		//Element buffer

	//The number of the vertices and indices in the mesh
	unsigned int m_VerticesCount, m_IndicesCount;
};
