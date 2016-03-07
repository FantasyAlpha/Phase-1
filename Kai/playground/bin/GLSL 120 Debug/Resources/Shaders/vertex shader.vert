#version 330

//NOTE(kai): location of the attributes
layout (location = 0) in vec3 position;	
layout (location = 2) in vec2 inputTexCoords;		

//NOTE(kai): "out" takes values out of the vertex shader and into the fragment shader 
out vec2 outputTexCoords;

uniform mat4 modelMatrix;

void main()
{
	//Set vertex position
	gl_Position = modelMatrix * vec4(position, 1.0f);
	outputTexCoords = inputTexCoords;
}