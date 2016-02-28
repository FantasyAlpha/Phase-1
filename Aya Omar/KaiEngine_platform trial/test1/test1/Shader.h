/*=======================================================================================================================
NOTE(kai): This file can:

1) creates and compile vertex and fragment shader
2) Activate the shader for drawing
=======================================================================================================================*/

#pragma once

#include <GL\glew.h>
#include <iostream>
#include <stdio.h>

class Shader{
private:
	enum ShaderType
	{
		FRAGMENT_SHADER = 0,
		VERTEX_SHADER = 1
	};

public:
	//Default constructor
	Shader() = default;
	//Pass the shader path to create the shader automatically
	Shader(char *vertexPath, char *fragmentPath);

	~Shader(){}

public:
	//Activate the shader (We need to do this before we begin drawing)
	void Activate();

	//TODO(kai): implement a function that can add and bind attributes
	//Add a new attribute in the shader
	void AddAttribute(char *name){}

private:
	//NOTE(kai): We create a shader by:
	//					1) Generating a handle for the shader
	//					2) Storing the shader code into the shader
	//					3) Compiling the shader
	//					4) Check for compilation errors
	//					5) Create a program handle
	//					6) Attach the shader to the program
	//					7) Link all the shaders in the program
	//					8) Check for link errors

	//Create a shader  
	void CreateShader(char *vertexPath, char *fragmentPath);

	//Add a shader into the program  
	void AddShader(char *path, ShaderType type);

	//Compile the shader we created
	void CompileShader(unsigned int shader);

	//Link the shaders
	void LinkShaders();

	//Print out the errors in compiling the vertex and fragment shader
	void GetCompileError(unsigned int shader);

	//Print out the errors in linking the vertex and fragment shader into a program
	void GetLinkError();

	//Return the contents of a text file
	char* LoadFromFile(char *path);

private:
	//Handle for the program (after we compile the shaders we link them in one program)
	unsigned int m_ProgramHandle;
};
