#include "Mesh.h"

//Create the buffers and store the mesh data in them
void LoadMesh(Mesh *mesh, Vertex *vertices, unsigned int verticesCount, unsigned int *indices, unsigned int indicesCount)
{
	mesh->VerticesCount = verticesCount;
	mesh->IndicesCount = indicesCount;

	//Use only with newer GLSL versions
#if GLSL_VERSION == MODERN_VERSION
	//Generate a handle to a vertex array object
	glGenVertexArrays(1, &mesh->VAO);
#endif

	//Generate a handle to a vertex buffer object
	glGenBuffers(1, &mesh->VBO);
	
	//Generate a handle to a element buffer object
	glGenBuffers(1, &mesh->EBO);

	//Bind the vertex array object (it will begin storing what is happening below
	BindMesh(mesh);

	//Create a Vertex buffer
	InitVBO(mesh, vertices);

	//Create an Element buffer
	InitEBO(mesh, indices);

	//Unbind everything
	UnbindMesh(mesh);
}

//Create a Vertex buffer and stores the vertices in it 
file_internal void InitVBO(Mesh *mesh, Vertex *vertices)
{
	//Bind the vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);

	//Store the data in the buffer
	glBufferData(GL_ARRAY_BUFFER, mesh->VerticesCount * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	//Enable the attribute with the given index in the shader to be used in rendering
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	//Defines where the attribute with the given index should look in buffer
	glVertexAttribPointer(0				//The attribute index
						, 3				//Number of elements
						, GL_FLOAT		//Type of elements
						, GL_FALSE		//Normalized or not
						, sizeof(Vertex)//Size of the stride (the next location that the pointer will go to)
						, 0);			//The pointer offset

	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) sizeof(Position));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) (sizeof(Position) + sizeof(Color)));
}

//Create an Element buffer and stores the indices in it 
file_internal void InitEBO(Mesh *mesh, unsigned int *indices)
{
	//Bind the element buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);

	//Store the data in the buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->IndicesCount * sizeof(unsigned int), indices, GL_STATIC_DRAW);
}

//Bind the buffers
file_internal void BindMesh(Mesh *mesh)
{
	//Use only with newer GLSL versions
#if GLSL_VERSION == MODERN_VERSION
	//Bind the element buffer
	glBindVertexArray(mesh->VAO);

#elif GLSL_VERSION == ANCIENT_VERSION	//Use with older GLSL versions
	//Bind the vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);

	//Enable the attribute with the given index in the shader to be used in rendering
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	//Defines where the attribute with the given index should look in buffer
	glVertexAttribPointer(0				//The attribute index
		, 3				//Number of elements
		, GL_FLOAT		//Type of elements
		, GL_FALSE		//Normalized or not
		, sizeof(Vertex)//Size of the stride (the next location that the pointer will go to)
		, 0);			//The pointer offset

	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) sizeof(Position));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(sizeof(Position) + sizeof(Color)));

	//Bind the vertex buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
#endif
}

//Unbind the buffers
file_internal void UnbindMesh(Mesh *mesh)
{
	//Use only with newer GLSL versions
#if GLSL_VERSION == MODERN_VERSION
	//Unbind the vertex array object
	glBindVertexArray(0);
#endif
	//Unbind the vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	//Unbind the element buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

//Draw the mesh
void DrawMesh(Mesh *mesh)
{
	//Bind the buffers
	BindMesh(mesh);

	BindTexture(mesh);

	//Draw the mesh
	glDrawElements(GL_TRIANGLES		//Drawing type
				 , mesh->IndicesCount	//Number of indices
				 , GL_UNSIGNED_INT	//Indices type
				 , 0);				//The location of the indices (NULL means to look for them in the buffer) 

	//Unbind the buffers
	UnbindMesh(mesh);
	UnbindTexture(mesh);
}

////
void SetTexture(Mesh *mesh, char *imagePath)
{
	std::vector<unsigned char> image;
	DataFile file;
	LoadFile(imagePath, &file);
	unsigned char *data = (unsigned char*)file.Data;
	decodePNG(image, mesh->TextureWidth, mesh->TextureHeight, (unsigned char*)file.Data, file.Length);

	glGenTextures(1, &mesh->TextureHandle);

	glBindTexture(GL_TEXTURE_2D, mesh->TextureHandle);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mesh->TextureWidth, mesh->TextureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(image[0]));

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void BindTexture(Mesh *mesh)
{
	if (mesh->TextureHandle)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->TextureHandle);
	}
}

void UnbindTexture(Mesh *mesh)
{
	if (mesh->TextureHandle)
	{		
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

////
Mesh* CreateSprite(int width, int height, int pos, char *imagePath, Color color)
{
	return NULL;
}