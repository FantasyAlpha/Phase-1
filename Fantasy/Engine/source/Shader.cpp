#include "Shader.h"

//Create a vertex and fragment shader  
Shader CreateShader(char *vertexPath, char *fragmentPath, char **attributeLocations, uint32 attributeLocationCount)
{
	bool res = ReloadGlew();

	Shader shader = {};

	//Create a handle for the program
	shader.ProgramHandle = glCreateProgram();

	//Add vertex shader to the program
	AddShader(&shader, vertexPath, VERTEX_SHADER);

	//Add fragment shader to the program
	AddShader(&shader, fragmentPath, FRAGMENT_SHADER);

	if (attributeLocations)
	{
		for (uint32 i = 0; i < attributeLocationCount; i++)
		{
			glBindAttribLocation(shader.ProgramHandle, i, attributeLocations[i]);
		}
	}

	//Link the shaders in the program and check for errors
	LinkShaders(&shader);

	return shader;
}

//Add a shader into the program  
void AddShader(Shader *shader, char *path, ShaderType type)
{
	//Handle for the shader
	unsigned int shaderHandle;

	//Load the shader code from a file
	DataFile shaderCode = {};
	LoadFile(path, &shaderCode);
	char *code = (char*)shaderCode.Data;
	

	//Create a handle for the shader (according to the shader type
	if (type == VERTEX_SHADER)
	{
		shaderHandle = glCreateShader(GL_VERTEX_SHADER);
	}
	else if (type == FRAGMENT_SHADER){
		shaderHandle = glCreateShader(GL_FRAGMENT_SHADER);
	}

	//Put the shader code in the shader
	//NOTE(kai): glShaderSource can take an array of strings and can also specify 
	//			the length of each string	
	glShaderSource(shaderHandle			//The shader handle
				 , 1				//The number of strings in the array 
				 , &code			//The array of strings containing shader code
				 , NULL);			//The length of each string ,null means that the strings will be NULL terminated (default size)

	//Compile the shader and check for errors
	CompileShader(shaderHandle);

	//Attach the shader to the program
	glAttachShader(shader->ProgramHandle, shaderHandle);

	//Delete the shader handle
	glDeleteShader(shaderHandle);

	UnloadFile(&shaderCode);
}

//Compile the shader we created
void CompileShader(unsigned int shader)
{
	//Compile the shader
	glCompileShader(shader);

	//Check for errors
	GetCompileError(shader);
}

//Link the shaders
void LinkShaders(Shader *shader)
{
	//Link all shaders in one program
	glLinkProgram(shader->ProgramHandle);

	//Check for link errors
	GetLinkError(shader);
}

//Print out the errors in compiling the vertex and fragment shader
void GetCompileError(unsigned int shader)
{
	//The compile status
	int success;

	//The error log
	char log[512];

	//Get the compile status
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	//If success is NULL then compiling the shader failed
	if (!success)
	{
		//Store the log file
		glGetShaderInfoLog(shader, 512, NULL, log);

		//Print out the log file
		std::cout << "Failed to compile shader:\n" << log << std::endl;
		system("PAUSE");
	}
}

//Print out the errors in linking the vertex and fragment shader into a program
void GetLinkError(Shader *shader)
{
	//The compile status
	int success;

	//The error log
	char log[512];

	//Get the link status
	glGetProgramiv(shader->ProgramHandle, GL_LINK_STATUS, &success);

	//If success is NULL then linking the shader to the program failed
	if (!success)
	{
		//Store the log file
		glGetProgramInfoLog(shader->ProgramHandle, 512, NULL, log);

		//Print out the log file
		std::cout << "Failed to link program:\n" << log << std::endl;
		system("PAUSE");
	}
}

//Activate the shader (We need to do this before we begin drawing)
void ActivateShader(Shader *shader)
{
	if (!shader)
	{
		glUseProgram(0);
	}
	glUseProgram(shader->ProgramHandle);
}

unsigned int GetUniformLocation(Shader *shader, UNIFORMS uniformType)
{
	return shader->UniformLocations[uniformType];
}

void AddUniform(Shader *shader, UNIFORMS type, char *name)
{
	shader->UniformLocations[type] = glGetUniformLocation(shader->ProgramHandle, name);
}