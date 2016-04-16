#include <Mesh.h>

static bool Glew_Initialized = false;

//Create the buffers and store the mesh data in them
Mesh CreateMesh(Vertex *vertices, unsigned int verticesCount, unsigned int *indices, unsigned int indicesCount, bool withNormals)
{
	Mesh mesh = {};
	if (!Glew_Initialized){
		bool res = ReloadGlew();
		Glew_Initialized = true;
	}
	
	mesh.IndicesCount = indicesCount;

	if (withNormals)
	{
		for (uint32 i = 0; i < indicesCount; i += 3)
		{
			uint32 i0 = indices[i];
			uint32 i1 = indices[i + 1];
			uint32 i2 = indices[i + 2];

			vec3 v1 = vertices[i1].Pos - vertices[i0].Pos;
			vec3 v2 = vertices[i2].Pos - vertices[i0].Pos;

			vec3 normal = v1.Cross(v2);
			normal.Normalize();

			vertices[i0].Normal += normal;
			vertices[i1].Normal += normal;
			vertices[i2].Normal += normal;

			vertices[i0].Normal.Normalize();
			vertices[i1].Normal.Normalize();
			vertices[i2].Normal.Normalize();
		}
	}

	//Use only with newer GLSL versions
#if GLSL_VERSION == MODERN_VERSION
	if (!buffers->VAO)
	{
		//Generate a handle to a vertex array object
		glGenVertexArrays(1, &buffers->VAO);
	}	
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

	/***********************************************************************************************************/
	//Create an Element buffer
	//NOTE(kai): We create the element buffer by:
	//					1) Create a handle for the buffer
	//					2) Bind the buffer
	//					3) Store the data in the buffer
	/***********************************************************************************************************/

	if (!mesh.VBO)
	{
		//Generate a handle to a vertex buffer object
		glGenBuffers(1, &mesh.VBO);
	}

	if (!mesh.EBO)
	{
		//Generate a handle to a element buffer object
		glGenBuffers(1, &mesh.EBO);
	}	
	
	BindMesh(&mesh);
	
	//Store the data in the buffer
	glBufferData(GL_ARRAY_BUFFER, verticesCount * sizeof(Vertex), vertices, GL_STATIC_DRAW);
	
	//Store the data in the buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesCount * sizeof(unsigned int), indices, GL_STATIC_DRAW);	

	/***********************************************************************************************************/
	/***********************************************************************************************************/

	//Unbind everything
	UnbindMesh();

	return mesh;
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

	if (mesh->WithNormals)
	{
		glEnableVertexAttribArray(ATTRIBUTE_INDEX::NORMALS);
		glVertexAttribPointer(ATTRIBUTE_INDEX::NORMALS, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(sizeof(vec3) + sizeof(vec2)));
	}

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

	//Draw the mesh
	glDrawElements(GL_TRIANGLES		//Drawing type
				 , mesh->IndicesCount	//Number of indices
				 , GL_UNSIGNED_INT	//Indices type
				 , 0);				//The location of the indices (NULL means to look for them in the buffer) 

	UnbindMesh();
}

////
Mesh CreateSprite(vec3 pos, vec2 size)
{
	Mesh sprite = {};

	//Square vertices 
	Vertex vertices[] =
	{
		Vertex{vec3(pos.x			, pos.y			  , pos.z), vec2(1, 1)},		//TOP RIGHT
		Vertex{vec3(pos.x			, pos.y - (size.y), pos.z), vec2(1, 0)},		//BOTTOM RIGHT
		Vertex{vec3(pos.x + (size.x), pos.y - (size.y), pos.z), vec2(0, 0)},		//BOTTOM LEFT
		Vertex{vec3(pos.x + (size.x), pos.y			  , pos.z), vec2(0, 1)},		//TOP LEFT
	};


	//Order of vertices that will be drawn
	unsigned int indices[] =
	{
		0, 1, 3,
		1, 2, 3,
	};

	sprite = CreateMesh(vertices, ArrayCount(vertices), indices, ArrayCount(indices));

	return sprite;
}

Mesh CreateCube(vec3 pos, vec3 size)
{
	Vertex vertices[24] =
	{
		Vertex{vec3(pos.x		  , pos.y		  , pos.z), vec2(0, 0)},
		Vertex{vec3(pos.x + size.x, pos.y		  , pos.z), vec2(1, 0)},
		Vertex{vec3(pos.x + size.x, pos.y + size.y, pos.z), vec2(1, 1)},
		Vertex{vec3(pos.x		  , pos.y + size.y, pos.z), vec2(0, 1)},
			  
		Vertex{vec3(pos.x		  , pos.y + size.y, pos.z		  ), vec2(0, 0)},
		Vertex{vec3(pos.x + size.x, pos.y + size.y, pos.z		  ), vec2(1, 0)},
		Vertex{vec3(pos.x + size.x, pos.y + size.y, pos.z - size.z), vec2(1, 1)},
		Vertex{vec3(pos.x		  , pos.y + size.y, pos.z - size.z), vec2(0, 1)},
			  
		Vertex{vec3(pos.x		  , pos.y		  , pos.z - size.z), vec2(0, 0)},
		Vertex{vec3(pos.x + size.x, pos.y		  , pos.z - size.z), vec2(1, 0)},
		Vertex{vec3(pos.x + size.x, pos.y + size.y, pos.z - size.z), vec2(1, 1)},
		Vertex{vec3(pos.x		  , pos.y + size.y, pos.z - size.z), vec2(0, 1)},
			  
		Vertex{vec3(pos.x		  , pos.y, pos.z	     ), vec2(0, 0)},
		Vertex{vec3(pos.x + size.x, pos.y, pos.z		 ), vec2(1, 0)},
		Vertex{vec3(pos.x + size.x, pos.y, pos.z - size.z), vec2(1, 1)},
		Vertex{vec3(pos.x		  , pos.y, pos.z - size.z), vec2(0, 1)},
			  														  
		Vertex{vec3(pos.x, pos.y		 , pos.z - size.z), vec2(0, 0)},
		Vertex{vec3(pos.x, pos.y		 , pos.z		 ), vec2(1, 0)},
		Vertex{vec3(pos.x, pos.y + size.y, pos.z		 ), vec2(1, 1)},
		Vertex{vec3(pos.x, pos.y + size.y, pos.z - size.z), vec2(0, 1)},
			  
		Vertex{vec3(pos.x + size.x, pos.y		  , pos.z - size.z), vec2(0, 0)},
		Vertex{vec3(pos.x + size.x, pos.y		  , pos.z		  ), vec2(1, 0)},
		Vertex{vec3(pos.x + size.x, pos.y + size.y, pos.z		  ), vec2(1, 1)},
		Vertex{vec3(pos.x + size.x, pos.y + size.y, pos.z - size.z), vec2(0, 1)},
	};

	uint32 indices[36] =
	{
		0, 1, 2,
		2, 0, 3,

		0 + 4, 1 + 4, 2 + 4,
		2 + 4, 0 + 4, 3 + 4,

		0 + 8, 1 + 8, 2 + 8,
		2 + 8, 0 + 8, 3 + 8,

		0 + 12, 1 + 12, 2 + 12,
		2 + 12, 0 + 12, 3 + 12,

		0 + 16, 1 + 16, 2 + 16,
		2 + 16, 0 + 16, 3 + 16,

		0 + 20, 1 + 20, 2 + 20,
		2 + 20, 0 + 20, 3 + 20,
	};

	Mesh result = CreateMesh(vertices, ArrayCount(vertices), indices, ArrayCount(indices));

	return result;
}