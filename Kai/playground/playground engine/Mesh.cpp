#include "Mesh.h"

static bool Glew_Initialized = false;

//Create the buffers and store the mesh data in them
void LoadMesh(MeshBuffers *buffers, MeshData *data, Vertex *vertices, unsigned int verticesCount, unsigned int *indices, unsigned int indicesCount)
{
	if (!Glew_Initialized){
		bool res = ReloadGlew();
		Glew_Initialized = true;
	}
	data->VerticesCount = verticesCount;
	data->IndicesCount = indicesCount;

	data->Vertices = vertices;
	data->Indices = indices;

	//Use only with newer GLSL versions
#if GLSL_VERSION == MODERN_VERSION
	//Generate a handle to a vertex array object
	glGenVertexArrays(1, &buffers->VAO);
#endif

	//Generate a handle to a vertex buffer object
	glGenBuffers(1, &buffers->VBO);
	
	//Generate a handle to a element buffer object
	glGenBuffers(1, &buffers->EBO);

	//Bind the vertex array object (it will begin storing what is happening below
	BindMesh(buffers);

	//Create a Vertex buffer
	InitVBO(buffers, data);

	//Create an Element buffer
	InitEBO(buffers, data);

	//Unbind everything
	UnbindMesh();
}

//Create a Vertex buffer and stores the vertices in it 
file_internal void InitVBO(MeshBuffers *buffers, MeshData *data)
{
	//Bind the vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, buffers->VBO);

	//Store the data in the buffer
	glBufferData(GL_ARRAY_BUFFER, data->VerticesCount * sizeof(Vertex), data->Vertices, GL_STATIC_DRAW);

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
file_internal void InitEBO(MeshBuffers *buffers, MeshData *data)
{
	//Bind the element buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers->EBO);

	//Store the data in the buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, data->IndicesCount * sizeof(unsigned int), data->Indices, GL_STATIC_DRAW);
}

//Bind the buffers
file_internal void BindMesh(MeshBuffers *buffers)
{
	//Use only with newer GLSL versions
#if GLSL_VERSION == MODERN_VERSION
	//Bind the element buffer
	glBindVertexArray(buffers->VAO);

#elif GLSL_VERSION == ANCIENT_VERSION	//Use with older GLSL versions
	//Bind the vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, buffers->VBO);

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
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers->EBO);
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
	BindMesh(&mesh->Buffers);

	BindTexture(&mesh->MeshTexture);

	//Draw the mesh
	glDrawElements(GL_TRIANGLES		//Drawing type
				 , mesh->Data.IndicesCount	//Number of indices
				 , GL_UNSIGNED_INT	//Indices type
				 , 0);				//The location of the indices (NULL means to look for them in the buffer) 

	//Unbind the buffers
	UnbindMesh();
	UnbindTexture();
}

////
void CreateSprite(Mesh *sprite, vec2 size, vec3 pos, Texture *texture, Color *colors, int colorCount)
{
	//Square vertices 
	Vertex vertices[] =
	{
		Vertex(Position(pos.x			, pos.y			  , pos.z), colorCount == 1 ? *colors : colors[0], TexCoords(1, 1)),		//TOP RIGHT
		Vertex(Position(pos.x			, pos.y + (size.y), pos.z), colorCount == 1 ? *colors : colors[1], TexCoords(1, 0)),		//BOTTOM RIGHT
		Vertex(Position(pos.x + (size.x), pos.y + (size.y), pos.z), colorCount == 1 ? *colors : colors[2], TexCoords(0, 0)),		//BOTTOM LEFT
		Vertex(Position(pos.x + (size.x), pos.y			  , pos.z), colorCount == 1 ? *colors : colors[3], TexCoords(0, 1)),		//TOP LEFT
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

	LoadMesh(&sprite->Buffers, &sprite->Data, vertices, sizeof(vertices) / sizeof(Vertex), indices, sizeof(indices) / sizeof(unsigned int));
	sprite->MeshTexture = *texture;
}