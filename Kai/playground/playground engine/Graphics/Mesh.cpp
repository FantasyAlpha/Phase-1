#include "Mesh.h"

static bool Glew_Initialized = false;

//Create the buffers and store the mesh data in them
void LoadMesh(Mesh *mesh, Vertex *vertices, unsigned int verticesCount, unsigned int *indices, unsigned int indicesCount)
{
	if (!Glew_Initialized){
		bool res = ReloadGlew();
		Glew_Initialized = true;
	}
	
	mesh->IndicesCount = indicesCount;

	//Use only with newer GLSL versions
#if GLSL_VERSION == MODERN_VERSION
	if (!buffers->VAO)
	{
		//Generate a handle to a vertex array object
		glGenVertexArrays(1, &buffers->VAO);
	}	
#endif

	if (!mesh->VBO)
	{
		//Generate a handle to a vertex buffer object
		glGenBuffers(1, &mesh->VBO);
	}

	if (!mesh->EBO)
	{
		//Generate a handle to a element buffer object
		glGenBuffers(1, &mesh->EBO);
	}

#if GLSL_VERSION == MODERN_VERSION
	//Generate a handle to a vertex array object
	glBindVertexArray(buffers->VAO);
#endif

	/***********************************************************************************************************/
	//Create a Vertex buffer
	//NOTE(kai): We create the vertex buffer by:
	//					1) Create a handle for the buffer
	//					2) Bind the buffer
	//					3) Store the data in the buffer
	//					4) Specify the vertex layout by enabling the attributes in the shader
	//						,set the attribute pointer

	/***********************************************************************************************************/
	
	//Bind the vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);

	//Store the data in the buffer
	glBufferData(GL_ARRAY_BUFFER, verticesCount * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	//Enable the attribute with the given index in the shader to be used in rendering
	glEnableVertexAttribArray(ATTRIBUTE_INDEX::POSITION);
	glEnableVertexAttribArray(ATTRIBUTE_INDEX::TEXTURE_COORDINATES);

	//Defines where the attribute with the given index should look in buffer
	glVertexAttribPointer(ATTRIBUTE_INDEX::POSITION				//The attribute index
		, 3				//Number of elements
		, GL_FLOAT		//Type of elements
		, GL_FALSE		//Normalized or not
		, sizeof(Vertex)//Size of the stride (the next location that the pointer will go to)
		, 0);			//The pointer offset

	glVertexAttribPointer(ATTRIBUTE_INDEX::TEXTURE_COORDINATES, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(sizeof(vec3)));

	/***********************************************************************************************************/
	/***********************************************************************************************************/

	/***********************************************************************************************************/
	//Create an Element buffer
	//NOTE(kai): We create the element buffer by:
	//					1) Create a handle for the buffer
	//					2) Bind the buffer
	//					3) Store the data in the buffer

	/***********************************************************************************************************/
	
	//Bind the element buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
	
	//Store the data in the buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesCount * sizeof(unsigned int), indices, GL_STATIC_DRAW);	

	/***********************************************************************************************************/
	/***********************************************************************************************************/

	//Unbind everything
	UnbindMesh();
}

//Bind the buffers
file_internal void BindMesh(Mesh *mesh)
{
	//Use only with newer GLSL versions
#if GLSL_VERSION == MODERN_VERSION
	//Bind the element buffer
	glBindVertexArray(buffers->VAO);

#elif GLSL_VERSION == ANCIENT_VERSION	//Use with older GLSL versions
	//Bind the vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);

	//Enable the attribute with the given index in the shader to be used in rendering
	glEnableVertexAttribArray(ATTRIBUTE_INDEX::POSITION);
	glEnableVertexAttribArray(ATTRIBUTE_INDEX::TEXTURE_COORDINATES);

	//Defines where the attribute with the given index should look in buffer
	glVertexAttribPointer(ATTRIBUTE_INDEX::POSITION				//The attribute index
		, 3				//Number of elements
		, GL_FLOAT		//Type of elements
		, GL_FALSE		//Normalized or not
		, sizeof(Vertex)//Size of the stride (the next location that the pointer will go to)
		, 0);			//The pointer offset

	glVertexAttribPointer(ATTRIBUTE_INDEX::TEXTURE_COORDINATES, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(sizeof(vec3)));

	//Bind the vertex buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
#endif
}

//Unbind the buffers
file_internal void UnbindMesh()
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

	//BindTexture(&mesh->MeshTexture);

	//Draw the mesh
	glDrawElements(GL_TRIANGLES		//Drawing type
				 , mesh->IndicesCount	//Number of indices
				 , GL_UNSIGNED_INT	//Indices type
				 , 0);				//The location of the indices (NULL means to look for them in the buffer) 

	UnbindMesh();
}

////
Mesh CreateSprite(vec2 size, vec3 pos)
{
	Mesh sprite = {};

	//Square vertices 
	Vertex vertices[] =
	{
		Vertex(vec3(pos.x			, pos.y			  , pos.z), vec2(1, 1)),		//TOP RIGHT
		Vertex(vec3(pos.x			, pos.y + (size.y), pos.z), vec2(1, 0)),		//BOTTOM RIGHT
		Vertex(vec3(pos.x + (size.x), pos.y + (size.y), pos.z), vec2(0, 0)),		//BOTTOM LEFT
		Vertex(vec3(pos.x + (size.x), pos.y			  , pos.z), vec2(0, 1)),		//TOP LEFT
	};


	//Order of vertices that will be drawn
	unsigned int indices[] =
	{
		0,
		1,
		3,
		1,
		2,
		3,
	};

	LoadMesh(&sprite, vertices, sizeof(vertices) / sizeof(Vertex), indices, sizeof(indices) / sizeof(unsigned int));

	return sprite;
}