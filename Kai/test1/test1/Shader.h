#pragma once

#include <GL\glew.h>
#include <iostream>
#include <stdio.h>

struct Shader{
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

	//A counter for the attributes in the shader
	//unsigned int m_AttributeCounter;

	//
};
