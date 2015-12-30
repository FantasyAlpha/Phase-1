#pragma once
#include<string>
// to use open gl functions 
#include<GL/glew.h>
#include<stdio.h>
class shader
{
public:
	// constructor parameter is the file  which will hold the shader program 
	shader(char* path1,char*path2);
	// binding functon to start/stop using the shaders 
	void bind();

	virtual ~shader();




private :
	void operator=(const shader& shader) {}
	shader(const shader& shader) {}
	// gl unsigned int to hold the address of the program (Handel) _ keep track of the program 

	// using GL data types instead of primitive types for handling multi  platforms  sizes 
	GLuint m_program;

	static const unsigned int num_shaders = 2;

	// array of shaders 1- vertex   2- fragment shader
	GLuint m_shaders[num_shaders];

	// return shader file as a string to use it by the GPU
	char* LoadShader(char* path);

	//check errors while linking the shader file and compiling 
	void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);


	// creating shader and return the handel (address)
	GLuint createShader(const std::string& text, unsigned int type);








};

