#include "Mesh.h"

//This constructor loads the mesh automatically
Mesh::Mesh(Vertex *vertices, unsigned int verticesCount, unsigned int *indices, unsigned int indicesCount)
{
	m_VerticesCount = verticesCount;
	m_IndicesCount = indicesCount;

	LoadMesh(vertices, indices);
}

//Create the buffers and store the mesh data in them
void Mesh::LoadMesh(Vertex *vertices, unsigned int *indices)
{
	//Use only with newer GLSL versions
#if GLSL_VERSION == MODERN_VERSION
	//Generate a handle to a vertex array object
	glGenVertexArrays(1, &m_VAO);
#endif

	//Generate a handle to a vertex buffer object
	glGenBuffers(1, &m_VBO);
	
	//Generate a handle to a element buffer object
	glGenBuffers(1, &m_EBO);

	//Bind the vertex array object (it will begin storing what is happening below
	Bind();

	//Create a Vertex buffer
	InitVBO(vertices);

	//Create an Element buffer
	InitEBO(indices);

	//Unbind everything
	Unbind();
}

//Create a Vertex buffer and stores the vertices in it 
void Mesh::InitVBO(Vertex *vertices)
{
	//Bind the vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

	//Store the data in the buffer
	glBufferData(GL_ARRAY_BUFFER, m_VerticesCount * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	//Enable the attribute with the given index in the shader to be used in rendering
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	//Defines where the attribute with the given index should look in buffer
	glVertexAttribPointer(0				//The attribute index
						, 3				//Number of elements
						, GL_FLOAT		//Type of elements
						, GL_FALSE		//Normalized or not
						, sizeof(Vertex)//Size of the stride (the next location that the pointer will go to)
						, 0);			//The pointer offset

	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) sizeof(Position));
}

//Create an Element buffer and stores the indices in it 
void Mesh::InitEBO(unsigned int *indices)
{
	//Bind the element buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

	//Store the data in the buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_IndicesCount * sizeof(unsigned int), indices, GL_STATIC_DRAW);
}

//Bind the buffers
void Mesh::Bind()
{
	//Use only with newer GLSL versions
#if GLSL_VERSION == MODERN_VERSION
	//Bind the element buffer
	glBindVertexArray(m_VAO);
#elif GLSL_VERSION == ANCIENT_VERSION	//Use with older GLSL versions
	//Bind the vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

	//Enable the attribute with the given index in the shader to be used in rendering
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	//Defines where the attribute with the given index should look in buffer
	glVertexAttribPointer(0				//The attribute index
		, 3				//Number of elements
		, GL_FLOAT		//Type of elements
		, GL_FALSE		//Normalized or not
		, sizeof(Vertex)//Size of the stride (the next location that the pointer will go to)
		, 0);			//The pointer offset

	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) sizeof(Position));

	//Bind the vertex buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
#endif
}

//Unbind the buffers
void Mesh::Unbind()
{
	//Use only with newer GLSL versions
#if GLSL_VERSION == MODERN_VERSION
	//Unbind the vertex array object
	glBindVertexArray(m_VAO);
#endif
	//Unbind the vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	//Unbind the element buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

//Draw the mesh
void Mesh::Draw()
{
	//Bind the buffers
	Bind();

	//Draw the mesh
	glDrawElements(GL_TRIANGLES		//Drawing type
				 , m_IndicesCount	//Number of indices
				 , GL_UNSIGNED_INT	//Indices type
				 , 0);				//The location of the indices (NULL means to look for them in the buffer) 

	//Unbind the buffers
	Unbind();
}