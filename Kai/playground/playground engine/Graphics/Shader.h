/*=======================================================================================================================
NOTE(kai): This file can:

1) creates and compile vertex and fragment shader
2) Activate the shader for drawing
=======================================================================================================================*/

#pragma once

#include <GL\glew.h>
#include "..\Utilities\\Utilities.h"

enum UNIFORMS
{
	MAX_NUMBER = 100,
	MODEL_MATRIX = 0,
	VIEW_MATRIX = 1,
	PROJECTION_MATRIX = 2,
};

enum ShaderType
{
	FRAGMENT_SHADER = 0,
	VERTEX_SHADER = 1
};

struct Shader
{
	//Handle for the program (after we compile the shaders we link them in one program)
	unsigned int ProgramHandle;

	unsigned int UniformLocations[UNIFORMS::MAX_NUMBER];
};


//Activate the shader (We need to do this before we begin drawing)
void ActivateShader(Shader *shader);

unsigned int GetUniformLocation(Shader *shader, UNIFORMS uniformType);

void AddUniform(Shader *shader, UNIFORMS type, char *name);

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
Shader CreateShader(char *vertexPath, char *fragmentPath);

//Add a shader into the program  
void AddShader(Shader *shader, char *path, ShaderType type);

//Compile the shader we created
void CompileShader(unsigned int shader);

//Link the shaders
void LinkShaders(Shader *shader);

//Print out the errors in compiling the vertex and fragment shader
void GetCompileError(unsigned int shader);

//Print out the errors in linking the vertex and fragment shader into a program
void GetLinkError(Shader *shader);